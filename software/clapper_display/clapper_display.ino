/**
 * freec0de
 * J. Adams <jna@retina.net> 
 * 9/2020
 * 
 * An open source SMPTE clapper board, similar to commercial solutions.
 * Reads, writes, and displays SMPTE time code.
 * 
 */

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include <MD_UISwitch.h>
#include <MD_Menu.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include "LedControl.h"
#include "slate.h"
#include "menu.h"
#include "ltc.h"
#include "config.h"

#include <EEPROM.h>
#define CONFIG_VERSION 10
#define CONFIG_MAGIC 0x110ade 
#define MAX_CLAPS 16

#include <AceButton.h>
using namespace ace_button;

/* Globals  ------------------ */
/* button defines */
// Create 2 ButtonConfigs. The System ButtonConfig is not used.
ButtonConfig btnConfig1;

static AceButton buttonUp(&btnConfig1);
static AceButton buttonDown(&btnConfig1);
static AceButton buttonSelect(&btnConfig1);

int buttonPending = NO_BUTTONS_PENDING;
float frameRate = 30;
DIVISOR currentDivisor;

/* LTC Reader globals */
const word sync = 0xBFFC;        // Sync word to expect when running tape forward
uint8_t tc[10] = {0};            // ISR Buffer to store incoming bits
volatile uint8_t xtc[8] = {0};   // Buffer to store valid TC data - sync bytes
volatile uint8_t tcFlags = 0;    // Various flags used by ISR and main code
uint32_t uSeconds;               // ISR store of last edge change time
bool hasSeenValidTC = false;     // if we have ever gotten valid time code, this is true.

// onboard LED
bool led_state = false;

/* the current time */
TIMECODE currentTime;

/* remember these many claps in history */
TIMECODE clapHistory[MAX_CLAPS];
int historyPosition = MAX_CLAPS - 1;

/* initial state and config */
int state = STATE_HELLO;
CONFIG config;

// On the ESP8266, note that these pins are the GPIO number not the physical pin and that
// even though the device is a 5V device logic works fantastically fine on 3.3v!
LedControl lc=LedControl(PIN_DISP_DIN, PIN_DISP_CLK, PIN_DISP_CS, 1);

// Menu Headers --------
MD_Menu::value_t vBuf;  // interface buffer for values


const PROGMEM MD_Menu::mnuHeader_t mnuHdr[] =
{
  // this doesn't matter as we only have one line of output.
  // however, if the min/max header numbers are not set right here, the
  // menu will not advance!
  { 5, "MD_Menu", 5, 100, 0 },
};

// Menu Items ----------

/* when does slate time out */
const PROGMEM char listTout[] = "0|15|30|60|120";
const int toutToVal[] = { 0, 15, 30, 60, 120 };

/* amount of time in hours before reminder */
const PROGMEM char listFeed[] = "0|2|4|6|8";
const char feedToVal[] = { 0, 2, 4, 6, 8 };

/* if we should flash or not */
const PROGMEM char listFlash[] = "0|1|2|3|4|5";
const char flashToVal[] = { 0, 1, 2, 3, 4, 5 };

/* how long to hold for */
const PROGMEM char listHold[] = "0|5|15|30|60|120";
const char flashToHold[] = { 0, 5, 15, 30, 60, 120 };

/* frame rate */
const PROGMEM char listFrames[] = "23|24|25|29|29D|30";
const float framesToVal[] = { 23.97, 24, 25, 29.97, 29.97, 30 };
#define DROP_INDEX 4

/* END Globals  ------------------ */

/* prototypes */
void setupTimer();
void ICACHE_RAM_ATTR handleTCChange();
/* END prototypes ---------------- */

char *timecodeToString(TIMECODE *tc) {
  static char buf[20];

  sprintf(buf, "%02d:%02d:%02d%c%02d",
    tc->hours, 
    tc->minutes, 
    tc->seconds, 
    config.frameRate == DROP_INDEX ? ';' : ':', 
    tc->frames); 
  return buf;
}


void initConfig() {
  Serial.println("Config Init.");

  config.version = CONFIG_VERSION;
  config.displayBrightness = 2;

  // user settable
  config.frameRate = 5;
  config.internal = true;
  config.timeOut = 3;
  config.feed = 4;
  config.jamLock = false;
  config.flashHeld = 3;
  config.holdClap = 0;
  config.backlight = false;
  config.plusOne=false;
  config.magic = CONFIG_MAGIC;

  EEPROM.put(0, config);
  EEPROM.commit();
};

void unpackConfig() {
  // take values from the saved configuration and set appropriate globals.
  DIVISOR *newDivisor;
  
  frameRate = framesToVal[config.frameRate];
  newDivisor = getDivisorForRate(frameRate);

  if (newDivisor == NULL) {
    Serial.println("Invalid rate divisor - system will halt.\n");
    // halp?
  }
  
  memcpy(&currentDivisor, newDivisor, sizeof(DIVISOR));

  lc.setIntensity(0, config.displayBrightness);

}

/** TBD
 *  load and process configuration from EEPROM
 */
void loadConfig() {
  EEPROM.get(0, config);

  if (config.magic != CONFIG_MAGIC || config.version != CONFIG_VERSION) {
    if (config.magic != CONFIG_MAGIC) { Serial.println("bad magic");}
    if (config.version != CONFIG_VERSION) { Serial.println("version changed - init config"); }

    // we are not initialized.
    initConfig();
  } else {
    Serial.print("Config OK, version ");
    Serial.println(config.version);
  }

};

MD_Menu::value_t *mnuExit(MD_Menu::mnuId_t id, bool bGet) {
  state = STATE_TIMECODE;
};

MD_Menu::value_t *mnuSave(MD_Menu::mnuId_t id, bool bGet) {
  // apparently it isn't possible to run timers while saving.
  timer1_detachInterrupt();
  timer1_disable();
  detachInterrupt(digitalPinToInterrupt(PIN_TC_IN));

  EEPROM.put(0, config);
  EEPROM.commit();

  attachInterrupt(digitalPinToInterrupt(PIN_TC_IN), handleTCChange, CHANGE);  
  setupTimer();

  lc.setString(0,7, "stored.", 0);
  delay(250);
  state = STATE_TIMECODE;
};

/* menu handlers */
MD_Menu::value_t *mnuLValueRqst(MD_Menu::mnuId_t id, bool bGet)
{
  // Value request callback for list variables
  // Note that all use of list operations uses indicies. Not values.
  MD_Menu::value_t *r = &vBuf;
  
  if (bGet) {
    switch(id) {
      case 20:
        vBuf.value = config.frameRate;
      break;
      case 30:
        vBuf.value = config.feed;
      break;
      case 40:
        vBuf.value = config.timeOut;
      break;
      case 60:
        vBuf.value = config.flashHeld;
      break;
      case 70:
        vBuf.value = config.holdClap;
      break;
    }
  } else {
    switch(id) {
      case 20:
        config.frameRate = vBuf.value;
        // reset our timers if the frame rate changes
        hasSeenValidTC = false;
        unpackConfig();
      break;
      case 30:
        config.feed = vBuf.value;
      break;
      case 40:
        config.timeOut = vBuf.value;
      break;
      case 60:
        config.flashHeld = vBuf.value;
      break;
      case 70:
        config.holdClap = vBuf.value;
      break;
    }
  }

  return(r);
}

MD_Menu::value_t *mnuIValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for integers variables
{
  MD_Menu::value_t *r = &vBuf;

  // right now this is only used by brightness
  if (bGet) {
      vBuf.value = config.displayBrightness;
  } else {
      config.displayBrightness = vBuf.value;
      lc.setIntensity(0, config.displayBrightness);
  }

  return(r);
}

MD_Menu::value_t *mnuBValueRqst(MD_Menu::mnuId_t id, bool bGet)
{
  // Value request callback for boolean variable
  MD_Menu::value_t *r = &vBuf;

  // are we setting or getting?
  if (bGet) {
    switch(id) {
      case 10:
        vBuf.value = config.internal;
      break;
      case 50:
        vBuf.value = config.jamLock;
        // reset hasSeen if this config setting changes.
        if (vBuf.value == true) { 
          hasSeenValidTC = false;
        }
      break;
      case 80:
        vBuf.value = config.plusOne;
      break;
      case 100:
        // this is a fake boolean that always returns false.
        vBuf.value = false;
      break;
    }
  } else {
    switch(id) {
      case 10:
        config.internal = vBuf.value;
      break;
      case 50:
        config.jamLock = vBuf.value;
      break;
      case 80:
        config.plusOne = vBuf.value;
      break;
      case 85:
        if (vBuf.value == true) {
          lc.clearDisplay(0);
          lc.setString(0,7, "-RE5ET-", 0);
          delay(1000);
          
          /* disable the timers and interrupts or we will die during EEPROM save.*/
          timer1_detachInterrupt();
          timer1_disable();
          detachInterrupt(digitalPinToInterrupt(PIN_TC_IN));

          initConfig();
          unpackConfig();
          
          attachInterrupt(digitalPinToInterrupt(PIN_TC_IN), handleTCChange, CHANGE);  
          setupTimer();
        }
      break;
    }

  }
  
  return(r);
}

const PROGMEM MD_Menu::mnuItem_t mnuItm[] =
{
  // Starting (Root) menu
  { 5,  "brite", MD_Menu::MNU_INPUT, 5 },   // brightness
  { 10, "1ntG", MD_Menu::MNU_INPUT, 10 },   // Internal generation, defaults to off
  { 20, "Frt ", MD_Menu::MNU_INPUT, 20 },   // Frame Rate Selection
  { 30, "feed", MD_Menu::MNU_INPUT, 30 },   // feed demand
  { 40, "Tout", MD_Menu::MNU_INPUT, 40 },   // timeout
  { 50, "jloc", MD_Menu::MNU_INPUT, 50 },   // jam lock (no run w/o lock)
  { 60, "Flash", MD_Menu::MNU_INPUT, 60 },  // flash
  { 70, "Hold", MD_Menu::MNU_INPUT, 70 },   // hold
  { 80, "Plus1", MD_Menu::MNU_INPUT, 80 },  // plus one reader mode
  { 85 ,"init", MD_Menu::MNU_INPUT, 85 },   // fake boolean to init config
  { 90, "store", MD_Menu::MNU_INPUT, 90 },  // save and back to timecode
  { 95, "return", MD_Menu::MNU_INPUT, 95 }, // back to timecode
};

// Input Items ---------
const PROGMEM MD_Menu::mnuInput_t mnuInp[] =
{
  { 5,  "brte", MD_Menu::INP_INT,  mnuIValueRqst, 1, 1, 0, 7, 0, 10, nullptr },
  { 10, "1ntG", MD_Menu::INP_BOOL, mnuBValueRqst, 1, 0, 0, 0, 0, 0, nullptr },
  { 20, "Frt ", MD_Menu::INP_LIST, mnuLValueRqst, 5, 0, 0, 0, 0, 0, listFrames },
  { 30, "feed", MD_Menu::INP_LIST, mnuLValueRqst, 5, 0, 0, 0, 0, 0, listFeed },
  { 40, "Tout", MD_Menu::INP_LIST, mnuLValueRqst, 5, 0, 0, 0, 0, 0, listTout },
  { 50, "jloc", MD_Menu::INP_BOOL, mnuBValueRqst, 1, 0, 0, 0, 0, 0, nullptr },
  { 60, "Flsh", MD_Menu::INP_LIST, mnuLValueRqst, 6, 0, 0, 0, 0, 0, listFlash },
  { 70, "Hold", MD_Menu::INP_LIST, mnuLValueRqst, 6, 0, 0, 0, 0, 0, listHold },
  { 80, "Pls1", MD_Menu::INP_BOOL, mnuBValueRqst, 1, 0, 0, 0, 0, 0, nullptr },
  { 85, "init", MD_Menu::INP_BOOL, mnuBValueRqst, 1, 0, 0, 0, 0, 0, nullptr },
  { 90, "store", MD_Menu::INP_RUN, mnuSave,       0, 0, 0, 0, 0, 0, nullptr },
  { 95, "return", MD_Menu::INP_RUN, mnuExit,      0, 0, 0, 0, 0, 0, nullptr },
};

// bring it all together in the global menu object
MD_Menu M(navigation, display,  // user navigation and display
  mnuHdr, ARRAY_SIZE(mnuHdr),   // menu header data
  mnuItm, ARRAY_SIZE(mnuItm),   // menu item data
  mnuInp, ARRAY_SIZE(mnuInp));  // menu input data


/* LTC Reader */
// reference: http://www.philrees.co.uk/articles/timecode.htm
void ICACHE_RAM_ATTR handleTCChange() {
  uint32_t edgeTimeDiff = micros() - uSeconds;          // Get time difference between this and last edge
  uSeconds = micros();                                    // Store time of this edge
  // adding 10% slop here?
  if ((edgeTimeDiff < currentDivisor.uMin1) || 
      (edgeTimeDiff > currentDivisor.uMax0)) { // Drop out now if edge time not within bounds

      bitSet(tcFlags, tcFrameError);
    return;
  }
  
  //Serial.println(edgeTimeDiff);
  
  if (edgeTimeDiff > currentDivisor.uMax1)                // A zero bit arrived
  {
    if (bitRead(tcFlags, tcHalfOne) == 1){                // But we are expecting a 1 edge
      bitClear(tcFlags, tcHalfOne);
      clearBuffer(tc, sizeof(tc));
      return;
    }

    // 0 bit
    shiftRight(tc, sizeof(tc));                           // Rotate buffer right
    // Shift replaces top bit with zero so nothing else to do
    //bitClear(tc[0], 7);                                 // Reset the 1 bit in the buffer
  }
  else                                                    // Not zero so must be a 1 bit
  { // 1 bit
    if (bitRead(tcFlags, tcHalfOne) == 0){                // First edge of a 1 bit
      bitSet(tcFlags, tcHalfOne);                         // Flag we have the first half
      return;
    }
    // Second edge of a 1 bit
    bitClear(tcFlags, tcHalfOne);                         // Clear half 1 flag
    shiftRight(tc, sizeof(tc));                           // Rotate buffer right
  
    bitSet(tc[0], 7);                                     // Set the 1 bit in the buffer
  }

  // Congratulations, we have managed to read a valid 0 or 1 bit into buffer
  if (word(tc[0], tc[1]) == sync){                        // Last 2 bytes read = sync?
    bitClear(tcFlags, tcFrameError);                      // Clear framing error
    bitClear(tcFlags, tcOverrun);                         // Clear overrun error
    
    if (bitRead(tcFlags, tcForceUpdate) == 1){
      bitClear(tcFlags, tcValid);
    }
    
    if (bitRead(tcFlags, tcValid) == 1) {                  // Last TC not read
      bitSet(tcFlags, tcOverrun);                         // Flag overrun error
      return;                                             // Do nothing else
    }
    
    for (uint8_t x = 0; x < sizeof(xtc); x++){            // Copy buffer without sync word
      xtc[x] = tc[x + 2];
    }

#ifdef DEBUG_TC_SERIAL
    Serial.println("valid tc");
#endif
    // we have valid timecode so we can reset our timer, which will now be jam sync'd to the
    // timecode. Our frame timer should now match their frame timer give or take a few uS

    // there are some discussions here about 'being in frame sync with time code'
    // which as best I can tell involves starting the timer -after- the sync word
    // which is nearly exactly what we do here, give or take some code.

    // this device is an analyzer that has -lots- of information about error detection.
    // https://www.brainstormtime.com/OLD/um_sa1.pdf
    timer1_write(currentDivisor.cpuTicksPerFrame);

    bitSet(tcFlags, tcValid);                             // Signal valid TC

    // wink the led
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);

    hasSeenValidTC = true;
  }
}

void clearBuffer(uint8_t theArray[], uint8_t theArraySize){
  for (uint8_t x = 0; x < theArraySize - 1; x++){
    theArray[x] = 0;
  }
}

void shiftRight(uint8_t theArray[], uint8_t theArraySize){
  uint8_t x;
  for (x = theArraySize; x > 0; x--){
    uint8_t xBit = bitRead(theArray[x - 1], 0);
    theArray[x] = theArray[x] >> 1;
    theArray[x] = theArray[x] | (xBit << 7);
  }
  theArray[x] = theArray[x] >> 1;
}

/* ISR for timer tick */
/* TODO: Convert the increment to a function so that it can be reused for plusOne */
TIMECODE *getNextTimecode(TIMECODE tc) {
  // implement a frame clock on a 1000uS / 1mS / .001 second interrupt
  // we don't have incoming sync, let's do it ourselves. 
  // in 1mS how many frames went by? 
  static TIMECODE retTime;
  memcpy(&retTime, &tc, sizeof(TIMECODE));
  
  retTime.frames++;
  
  float maxFrames = frameRate;
  if (frameRate == 29.97) {
    // 29.97 time code is 30 fr/sec code with a rate of 29.97. 
    maxFrames = 30;
  }

  if (retTime.frames > maxFrames - 1) {
    retTime.frames = 0;
    retTime.seconds++;

    /* 
      If the seconds field is now 00 (that is, we are now in the first second of a
      minute in time code time), the range of the frames field is 02-29 (that is,
      the frame numbers 00 and 01 are skipped at the beginning of that
      second), except that:
      
      If the seconds field is now 00, but the minutes field now is evenly
      divisible by 10 (that includes a value of 00), then the range of the frames
      field is the normal 00-29 (that is, no frame numbers are skipped at the
      beginning of that second).
    */
    if (retTime.seconds > 59) { 
        retTime.seconds = 0;
        retTime.minutes++;
    }

    if (retTime.seconds == 0) { 
      retTime.frames = 2;

      if (retTime.minutes % 10 == 0 || retTime.minutes == 0) {
        retTime.frames = 0;
      } 
    }
  }

  if (retTime.minutes > 59) { 
    retTime.minutes = 0;
    retTime.hours++;
  }

  if (retTime.hours > 23) { 
    retTime.hours = 0;
    retTime.seconds = 0;
    retTime.minutes = 0;
    retTime.frames = 0;
  }

  return &retTime;
}

void ICACHE_RAM_ATTR onTimerISR(){
  if (!bitRead(tcFlags, tcValid)) {
    memcpy(&currentTime, getNextTimecode(currentTime), sizeof(TIMECODE));
  }
}

/**
 * show the current smpte time on the display.
 * @params {Integer} val number of milliseconds
 **/ 
void displayTimecode(TIMECODE *tc) {
  if (!hasSeenValidTC && config.jamLock) {
    lc.setString(0,7, "-loc'd- ", 0);
    return;
  }

  // setDigit: addr, digit, value, decimal point.

  // Hours
  lc.setDigit(0,7,TENS(tc->hours),false);
  lc.setDigit(0,6,ONES(tc->hours),true);

  // Minutes    
  lc.setDigit(0,5,TENS(tc->minutes),false);
  lc.setDigit(0,4,ONES(tc->minutes),true);

  // seconds
  lc.setDigit(0,3,TENS(tc->seconds),false);
  lc.setDigit(0,2,ONES(tc->seconds),true); // seems to set decimal point fine. 

  // frames
  lc.setDigit(0,1,TENS(tc->frames),false);
  lc.setDigit(0,0,ONES(tc->frames),state == STATE_HISTORY); // set last decimal place if in history mode.

  // Wink the LED once a second. (it appears to be active low?)
  // We try hard here not to write if we don't have to.

  /* TODO: wink the LED when we are generating. not always. 
  if (tc->frames < 5) { 
    digitalWrite(LED_BUILTIN, false);
  } else {
    digitalWrite(LED_BUILTIN, true);
  }
  */
}

void showFrameRate() {
  // show frame rate on right side of display. does not clear screen.
  lc.setDigit(0,3,TENS(frameRate),false);
  lc.setDigit(0,2,ONES(frameRate),false);

  if (config.frameRate == DROP_INDEX) { // 29.97 drop
    lc.setChar(0,0,'d',false);
  }
}

/**
 * Save the claps in history. Shift them all back one and put the
 * latest clap in to clapHistory[MAX_CLAPS]
 */
void saveClap() {
  for (int i=1; i < MAX_CLAPS; i++) {
    memcpy(&clapHistory[i-1], &clapHistory[i], sizeof(TIMECODE));
  }
  memcpy(&clapHistory[MAX_CLAPS-1], &currentTime, sizeof(TIMECODE));
}

void handleClapper() {
  // Handle the resistor divider network which only has one button for now.
  char buf[12];

  switch (state) {
    case STATE_TIMECODE:
      saveClap();
      Serial.print("Clap at ");
      sprintf(buf, "%s", timecodeToString(&clapHistory[MAX_CLAPS-1]));
      Serial.print(buf);
      Serial.print(" ");
      Serial.print(currentDivisor.frameRate);
      Serial.println(" fps");
      state = STATE_CLAP;
    break;
  }
}    

void handleButtonEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */)
{
  char buf[12];

  // generic button handler 
  switch (eventType) {
    case AceButton::kEventPressed:
    case AceButton::kEventRepeatPressed: {  
      uint8_t pin = button->getPin();

      switch (state) {
        case STATE_TIMECODE:
          switch(pin) { 
            case PIN_BTN_CLAPPER:
              saveClap();
              Serial.print("Clap at ");
              sprintf(buf, "%s", timecodeToString(&clapHistory[MAX_CLAPS-1]));
              Serial.print(buf);
              Serial.print(" ");
              Serial.print(currentDivisor.frameRate);
              Serial.println(" fps");
              state = STATE_CLAP;
            break;
            case PIN_BTN_SELECT:
              state = STATE_MENU;
              M.runMenu(true);
              break;
            case PIN_BTN_UP:
              dumpHistory();
              state = STATE_HISTORY;
              historyPosition = MAX_CLAPS - 1;
              showHistoryPosition();
              break;
          }
        break;
        case STATE_HISTORY:
          switch(pin) { 
            case PIN_BTN_UP:
              historyPosition--;
              if (historyPosition < 0) {
                historyPosition = MAX_CLAPS-1;
              }
              showHistoryPosition();
              break;
            case PIN_BTN_DOWN:
              historyPosition++;
              if (historyPosition > MAX_CLAPS-1) {
                historyPosition = 0;
              }
              showHistoryPosition();
              break;
            case PIN_BTN_SELECT:
              state = STATE_TIMECODE;
              break;
          }
        break;
        case STATE_MENU:
          // STATE_MENU is handled by MD_Menu here.
          buttonPending = pin;
        break;
      }
    }
  }
}

char *removeChar(char *str, char garbage) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';

    return str;
}

bool display(MD_Menu::userDisplayAction_t action, char *msg)
// Output display to a one of 2 line LCD display. 
// For a one line display, comment out the L0 handling code.
// The output display line is cleared with spaces before the
// requested message is shown.
{
  char msgCleaned[10];
  switch (action)
  {
   case MD_Menu::DISP_INIT:
     lc.clearDisplay(0);
   break;  

   case MD_Menu::DISP_CLEAR:
    lc.clearDisplay(0);
   break;
  /*
  Since we have only one line this doesn't matter. 

  case MD_Menu::DISP_L0:
    Serial.print("l0: [");
    Serial.print(msg);
    Serial.println("]");
  break;
  */
  case MD_Menu::DISP_L1:
    // get rid of a bunch of garbage from the library
    strcpy(msgCleaned, removeChar(msg, '<'));
    strcpy(msgCleaned, removeChar(msg, '>'));
    strcpy(msgCleaned, removeChar(msg, '['));
    strcpy(msgCleaned, removeChar(msg, ']'));
    lc.clearDisplay(0);
    lc.setString(0,7, msgCleaned, 0);
    break;
  }

  return(true);
}

MD_Menu::userNavAction_t navigation(uint16_t &incDelta)
{
  // it's always one move.
  incDelta = 1;

  // since our button handler is evented, we look at buttonPending
  // to see if we have anything to do.
  switch(buttonPending) {
    case PIN_BTN_UP:
      buttonPending = NO_BUTTONS_PENDING;
      return(MD_Menu::NAV_DEC);
    break;

    case PIN_BTN_DOWN:
      buttonPending = NO_BUTTONS_PENDING;
      return(MD_Menu::NAV_INC);
    break;

    case PIN_BTN_SELECT:
      buttonPending = NO_BUTTONS_PENDING;
      return(MD_Menu::NAV_SEL);
    break;

    default:
      // nothing to do, clear it.
      buttonPending = NO_BUTTONS_PENDING;
      return(MD_Menu::NAV_NULL);
  }
}

void setupTimer() {
  /* start the clock. we use hardware timer 1 and cannot use 0 -- 0 is used by WiFi */
  timer1_attachInterrupt(onTimerISR);

  /* Set up master timer
   *
   * The clock on the ESP8266 runs at 80Mhz (or 80 ticks/us).
   * We need very precise timing for frames, so let's use the timer to set up
   * a clock that exactly matches the frame rate.
   */
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);

  /*
   * Afaik the timers count down to zero and fire the interrupt at zero. 
   * timer1_write resets this countdown.
   */
  timer1_write(currentDivisor.cpuTicksPerFrame);
}

void setupLED() { 
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */   
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, config.displayBrightness);
  /* and clear the display */
  lc.clearDisplay(0);
}

void setupButtonsandPins() {

  btnConfig1.setEventHandler(handleButtonEvent);
  btnConfig1.setFeature(ButtonConfig::kFeatureClick);
  
  pinMode(PIN_BTN_SELECT, INPUT);
  buttonSelect.init(PIN_BTN_SELECT, HIGH, 0);
  
  pinMode(PIN_BTN_UP, INPUT);
  buttonUp.init(PIN_BTN_UP, HIGH, 1);

  pinMode(PIN_BTN_DOWN, INPUT);
  // since PIN_BTN_DOWN is zero, and the function is overloaded,
  // we need a cast to clear the compiler here. ugh.
  buttonDown.init((uint8_t)PIN_BTN_DOWN, LOW, 2);

  pinMode(LED_BUILTIN, OUTPUT);

  /* pins for timecode */
  pinMode(PIN_TC_IN, INPUT);
  pinMode(PIN_TC_OUT, OUTPUT);
}

void setupWiFi() { 
  Serial.println("Turning WiFi Off");
 // WiFi.mode(WIFI_OFF);    //This also works
}

void setup() {
  // Start serial
  Serial.begin(115200);
  while (! Serial); // wait for serial to settle.

  // kill WiFi
  setupWiFi();
  initTimecode(&currentTime);

  // Load Configuration from EEPROM.
  EEPROM.begin(sizeof(CONFIG));
  loadConfig();

  Serial.println("Config: unpack.");
  unpackConfig();
  Serial.println("Config: unpack ok.");

  /* LED Setup ---------------------------------- */
  setupLED();
  Serial.println("LED setup ok.");

  /* Master Clock ------------------------------- */
  Serial.print("Timer start with divisor ");
  Serial.print(currentDivisor.cpuTicksPerFrame);
  Serial.print(" (");
  Serial.print(currentDivisor.frameRate);
  Serial.println(" fps)");

  setupTimer();
  Serial.println("Timer is Go.");

  /* Buttons and Blinky ------------------------- */
  setupButtonsandPins();
  Serial.println("Buttons setup OK.");

  /* Setup the interrupt on TC_IN for LTC reading */
  attachInterrupt(digitalPinToInterrupt(PIN_TC_IN), handleTCChange, CHANGE);
  Serial.println("TC Interrupt ON.");

  /* menu */
  M.begin();
  M.setMenuWrap(true);
  M.setTimeout(MENU_TIMEOUT);
}

void showUserBits() {
  // show the user bits on the LEDs.
  char userBits[9];

  if (hasSeenValidTC) {
    userBits[0] = ((xtc[7] & 0xF0) >> 4) + '0';             // user bits 1
    userBits[1] = ((xtc[6] & 0xF0) >> 4) + '0';             // user bits 2  
            
    userBits[2] = ((xtc[5] & 0xF0) >> 4) + '0';             // user bits 3
    userBits[3] = ((xtc[4] & 0xF0) >> 4) + '0';             // user bits 4
        
    userBits[4] = ((xtc[3] & 0xF0) >> 4) + '0';             // user bits 5
    userBits[5] = ((xtc[2] & 0xF0) >> 4) + '0';             // user bits 6
              
    userBits[6] = ((xtc[1] & 0xF0) >> 4) + '0';             // user bits 7
    userBits[7] = ((xtc[0] & 0xF0) >> 4) + '0'; 
    userBits[8] = '\0';
    
    // show the string with all of the dots turned on
    lc.setString(0, 7,userBits, 0xff);
  } else {
    // since we've never sync'd before we don't have user bits. tell the user.
    lc.setString(0, 7, "00c0de", 0x00);
    lc.setDigit(0,1,TENS(frameRate),false);
    lc.setDigit(0,0,ONES(frameRate),false);
  }
}

void showHistoryPosition() {
  lc.clearDisplay(0);
  lc.setString(0,7, "-------", 0x0);
  lc.setDigit(0,2,TENS(historyPosition+1),false);
  lc.setDigit(0,1,ONES(historyPosition+1),false);
  delay(250);
}

/**
 * dumps history to the serial port
 */

void dumpHistory() {
  char buf[18];

  Serial.println();
  Serial.print(framesToVal[config.frameRate]);
  Serial.print(" fps, ");
  Serial.println(config.frameRate == DROP_INDEX  ? "Drop" : "Non-Drop");

  for (int i = 0; i < MAX_CLAPS; i++) {
    sprintf(buf, "%02d %s", i, timecodeToString(&clapHistory[i]));
    Serial.println(buf);
  }
}

void debounceClapper() {
  static uint16_t lastButtonState = 0;
  static long lastDebounceTime = 0;
  static uint16_t buttonState;

  long debounceDelay = 8;

  uint16_t reading = analogRead(PIN_BTN_CLAPPER);
#ifdef CALIBRATE_ANALOG
  // dump the A0 value. Right now with a 40k and 10k voltage divider, 
  // it reads about 865 on closure
  Serial.println(reading);
#endif
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debounce timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState > 500) {
        // we have a valid clap.
        handleClapper();
      }
    }
  }

  lastButtonState = reading;
}

void loop() {
  // check all the buttons for activity.
  buttonUp.check();
  buttonDown.check();
  buttonSelect.check();
  debounceClapper();

  // Do we have valid timecode yet?
  if (bitRead(tcFlags, tcValid) == 1) {
#ifdef DEBUG_TC_SERIAL
    char timeCode[12];               // For example code another buffer to write decoded timecode
    char userBits[12];

    timeCode[0] = (xtc[0] & 0x03) + '0';                    // 10's of hours
    timeCode[1] = (xtc[1] & 0x0F) + '0';                    // hours
    timeCode[2] = ':';                
    timeCode[3] = (xtc[2] & 0x07) + '0';                    // 10's of minutes
    timeCode[4] = (xtc[3] & 0x0F) + '0';                    // minutes
    timeCode[5] =  ':';              
    timeCode[6] = (xtc[4] & 0x07) + '0';                    // 10's of seconds
    timeCode[7] = (xtc[5] & 0x0F) + '0';                    // seconds
    timeCode[8] =  '.';              
    timeCode[9] = (xtc[6] & 0x03) + '0';                    // 10's of frames
    timeCode[10] = (xtc[7] & 0x0F) + '0';                   // frames
    
    userBits[0] = ((xtc[7] & 0xF0) >> 4) + '0';             // user bits 1
    userBits[1] = ((xtc[6] & 0xF0) >> 4) + '0';             // user bits 2  
    userBits[2] = '-';            
    userBits[3] = ((xtc[5] & 0xF0) >> 4) + '0';             // user bits 3
    userBits[4] = ((xtc[4] & 0xF0) >> 4) + '0';             // user bits 4
    userBits[5] = '-';            
    userBits[6] = ((xtc[3] & 0xF0) >> 4) + '0';             // user bits 5
    userBits[7] = ((xtc[2] & 0xF0) >> 4) + '0';             // user bits 6
    userBits[8] = '-';            
    userBits[9] = ((xtc[1] & 0xF0) >> 4) + '0';             // user bits 7
    userBits[10] = ((xtc[0] & 0xF0) >> 4) + '0';
                // user bits 8
    Serial.print(timeCode);
    Serial.print("\t");
    Serial.println(userBits); 
#endif // DEBUG_TC_SERIAL

    /* update current time */
    unsigned long tcHours  = ((xtc[0] & 0x03) * 10) + (xtc[1] & 0x0F);
    unsigned long tcMins   = ((xtc[2] & 0x07) * 10) + (xtc[3] & 0x0F);
    unsigned long tcSecs   = ((xtc[4] & 0x07) * 10) + (xtc[5] & 0x0F);
    unsigned long tcFrames = ((xtc[6] & 0x03) * 10) + (xtc[7] & 0x0F);

    currentTime.hours = tcHours;
    currentTime.minutes = tcMins;
    currentTime.seconds = tcSecs;
    currentTime.frames = tcFrames;

    // Finished with TC so signal to ISR it can overwrite it with next TC
    bitClear(tcFlags, tcValid);    
  }

  // like a video game, we have a state machine.
  switch(state) {
    case STATE_HELLO:
      // say hi!
      Serial.println("-=>: Welcome to freec0de! :<=-\n");
      lc.setString(0,7,"FREECODE", 0);
      delay(1000);
      lc.clearDisplay(0);
      lc.setString(0,7,"bld  1.0", 0);
      delay(500);
      lc.clearDisplay(0);
      lc.setString(0,7,"FP5", 0);
      showFrameRate();
      delay(500);
      lc.clearDisplay(0);

      state = STATE_TIMECODE;
      Serial.println("Timecode Mode");
      break;
    case STATE_CLAP:
      // clapper has closed
      displayTimecode(&clapHistory[MAX_CLAPS-1]);
      if (config.flashHeld > 0) {
        lc.setIntensity(0,8);
        delay(flashToVal[config.flashHeld] * 
              rateDivisors[config.frameRate].secPerFrame *
              1000);
              
        lc.setIntensity(0,config.displayBrightness);
      } else {
        // we'll hold for three frames 
        delay(3 * 
              rateDivisors[config.frameRate].secPerFrame * 
              1000);
      }
      // show user bits for one second. 
      showUserBits();
      delay(1000);

      // now if hold is non-zero, we'll hold for a bit
      if (config.holdClap > 0) {
        displayTimecode(&clapHistory[MAX_CLAPS-1]);
        delay(1000 * flashToHold[config.holdClap]);
      }

      lc.clearDisplay(0);
      delay(1000);
      state = STATE_TIMECODE;
      break;
    case STATE_TIMECODE:
      displayTimecode(&currentTime);
      break;
    case STATE_MENU:
      // handle timeouts. If menu times out we go back to TC
      if (M.isInMenu()) {
        M.runMenu();
      } else {
        state = STATE_TIMECODE;
      }
      break;
    case STATE_HISTORY:
      displayTimecode(&clapHistory[historyPosition]);
      break;
  }
  
}
