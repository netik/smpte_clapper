/**
 * freeslate
 * J. Adams <jna@retina.net> 
 * 9/2020
 * 
 * An open source SMPTE clapper board, similar to commercial solutions.
 * Reads, writes, and displays SMPTE time code.
 * 
 * TODO:
 *   - SMPTE timecode generation
 *   - Frame Rate Error Alarm (how?!!)
 *   - Menu system 
 *      - Int or Ext
 *      - Set Frame Rate
 *      - Set Display Brightness
 *      - Set modes
 * 
 *   - Save/load eeprom config
 * http://www.denecke.com/Support/Documents/TS-C_1013.pdf
 * Battery voltage and low battery warning readout
 * Time Out: Display times out when sticks are left open. 
 * EL Backlight: Enable/disable in low brightness.
 * Feed Alert: Reminds you when to jam. Hold Clap Frame: Displays the last time code after the user bits.
 * Jam Lock: Inhibits running time code without jamming.
 * Plus 1 FrameReader: Time code is displayed in real time when in read mode.
 * Flash Frame: Changes intensity to ensure exposure. 
 * Scroll back: Push Set button while sticks are closed to displayscroll back of previous claps.
 *    Scroll back memory is cleared at power off.
 */

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include <MD_UISwitch.h>
#include <MD_Menu.h>
#include <Ticker.h>
#include "LedControl.h"
#include "slate.h"
#include "menu.h"
#include "ltc.h"

#include <AceButton.h>
using namespace ace_button;

/* button defines */
AceButton buttonUp;
AceButton buttonDown;
AceButton buttonSelect;
AceButton buttonClapper;
int buttonPending = NO_BUTTONS_PENDING;

/* EEPROM config */
typedef struct configuration {
  // limit 512 bytes
  int version;
  int displayBrightness;
  int frameRate;
  char magic[4]; // used for checking if config is init'd
} CONFIG;

MD_Menu::value_t vBuf;  // interface buffer for values

// Menu Headers --------
const PROGMEM MD_Menu::mnuHeader_t mnuHdr[] =
{
  // this doesn't matter as we only have one line of output.
  { 10, "MD_Menu", 10, 15, 0 },
};

// Menu Items ----------
const PROGMEM MD_Menu::mnuItem_t mnuItm[] =
{
  // Starting (Root) menu
  { 10, "1nt/Ltc", MD_Menu::MNU_MENU, 11 },
  { 11, "Tout", MD_Menu::MNU_MENU, 11 },
  { 12, "Jloc", MD_Menu::MNU_MENU, 11 },
  { 13, "Flsh", MD_Menu::MNU_MENU, 11 },
  { 14, "Hold", MD_Menu::MNU_MENU, 11 },
  { 15, "Pls1", MD_Menu::MNU_MENU, 11 },
};

// Input Items ---------
const PROGMEM MD_Menu::mnuInput_t mnuInp[] =
{
  { 11, "Bool", MD_Menu::INP_BOOL, mnuBValueRqst, 1, 0, 0, 0, 0, 0, nullptr },

};

/* menu handlers */
MD_Menu::value_t *mnuIValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for integers variables
{
  MD_Menu::value_t *r = &vBuf;
}

MD_Menu::value_t *mnuBValueRqst(MD_Menu::mnuId_t id, bool bGet)
// Value request callback for boolean variable
{
  MD_Menu::value_t *r = &vBuf;
}



// bring it all together in the global menu object
MD_Menu M(navigation, display,        // user navigation and display
  mnuHdr, ARRAY_SIZE(mnuHdr), // menu header data
  mnuItm, ARRAY_SIZE(mnuItm), // menu item data
  mnuInp, ARRAY_SIZE(mnuInp));// menu input data

// On the ESP8266, note that these pins are the GPIO number not the physical pin and that
// even though the device is a 5V device logic works fantastically fine on 3.3v!
LedControl lc=LedControl(PIN_DISP_DIN, PIN_DISP_CLK, PIN_DISP_CS, 1);

/* Globals - System State */
bool  inDropMode = false;
float frameRate = 30;
int currentDivisor = getDivisorForRate(frameRate);

/* LTC Reader globals */
const word sync = 0xBFFC;        // Sync word to expect when running tape forward
uint8_t tc[10] = {0};            // ISR Buffer to store incoming bits
volatile uint8_t xtc[8] = {0};   // Buffer to store valid TC data - sync bytes
volatile uint8_t tcFlags = 0;    // Various flags used by ISR and main code
uint32_t uSeconds;               // ISR store of last edge change time
bool hasSeenValidTC = false;     // if we have ever gotten valid time code, this is true.

/* what are we doing? */
TIMECODE currentTime;
int state = STATE_HELLO;

/** TBD
 *  load and process configuration from EEPROM
 */
CONFIG *loadConfig() {
  return false;
};

/* LTC Reader */
void ICACHE_RAM_ATTR handleTCChange() {
  uint32_t edgeTimeDiff = micros() - uSeconds;            // Get time difference between this and last edge
  uSeconds = micros();                                    // Store time of this edge
  if ((edgeTimeDiff < uMin1) or (edgeTimeDiff > uMax0)) { // Drop out now if edge time not withing bounds
    bitSet(tcFlags, tcFrameError);
    return;
  }
  
  if (edgeTimeDiff > uMax1)                               // A zero bit arrived
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
      bitClear(tcFlags, tcValid);                         // Signal last TC read
    }
    if (bitRead(tcFlags, tcValid) == 1){                  // Last TC not read
      bitSet(tcFlags, tcOverrun);                         // Flag overrun error
      return;                                             // Do nothing else
    }
    for (uint8_t x = 0; x < sizeof(xtc); x++){            // Copy buffer without sync word
      xtc[x] = tc[x + 2];
    }
#ifdef DEBUG_TC_SERIAL
    Serial.println("valid tc");
#endif

    // we have valid timecode so we can reset our timer, which will now jam sync'd to the
    // timecode. Our frame timer should now match their frame timer give or take a few uS
    timer1_write(rateDivisors[currentDivisor].cpuTicksPerFrame);
    bitSet(tcFlags, tcValid);                             // Signal valid TC
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
void ICACHE_RAM_ATTR onTimerISR(){
  // implement a frame clock on a 1000uS / 1mS / .001 second interrupt
  // we don't have incoming sync, let's do it ourselves. 
  // in 1mS how many frames went by? 
  currentTime.frames++;

  if (currentTime.frames > frameRate-1) {
    currentTime.frames = 0;
    currentTime.seconds++;

    // when drop frame is enabled, every mimute starts at frame 2 instead of
    // at frame 0, unless the minute is divisible by 10.
    //
    // see: http://dougkerr.net/Pumpkin/articles/SMPTE-29.97DF.pdf

    if (inDropMode) { 
      if (currentTime.seconds > 59) { 
        // we're going into the next minute. 
        if ((currentTime.minutes + 1) % 10 != 0) { 
          currentTime.frames = 2;
        }
      }
    }
  }

  if (currentTime.seconds > 59) { 
      currentTime.seconds = 0;
      currentTime.minutes++;
  }

  if (currentTime.minutes > 59) { 
    currentTime.minutes = 0;
    currentTime.hours++;
  }

  if (currentTime.hours > 23) { 
    currentTime.hours = 0;
    currentTime.seconds = 0;
    currentTime.minutes = 0;
    currentTime.frames = 0;
  }

}

/**
 * show the current smpte time on the display.
 * @params {Integer} val number of milliseconds
 **/ 
void displayCurrentTime(TIMECODE *tc) {
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
    lc.setDigit(0,0,ONES(tc->frames),false);

    // Wink the LED once a second. (it appears to be active low?)
    // We try hard here not to write if we don't have to.

    if (tc->frames < (frameRate/2)) { 
      digitalWrite(LED_BUILTIN, false);
    } else {
      digitalWrite(LED_BUILTIN, true);
    }
}

void showFrameRate() {
  // show frame rate on right side of display. does not clear screen.
  lc.setDigit(0,3,TENS(frameRate),false);
  lc.setDigit(0,2,ONES(frameRate),false);

  if (inDropMode) {
    lc.setChar(0,0,'d',false);
  }
}

void handleButtonEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */)
{
  // generic button handler 
  switch (eventType) {
    case AceButton::kEventPressed:
    case AceButton::kEventRepeatPressed: {  
      uint8_t pin = button->getPin();
      Serial.println(pin);

      switch (state) {
        case STATE_TIMECODE:
          switch(pin) { 
            case PIN_BTN_CLAPPER:
              Serial.println("Clap");
              state = STATE_CLAP;
              break;
            case PIN_BTN_SELECT:
              Serial.println("Select");
              state = STATE_MENU;
              M.runMenu(true);
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

bool display(MD_Menu::userDisplayAction_t action, char *msg)
// Output display to a one of 2 line LCD display. 
// For a one line display, comment out the L0 handling code.
// The output display line is cleared with spaces before the
// requested message is shown.
{
  switch (action)
  {
   case MD_Menu::DISP_INIT:
     lc.clearDisplay(0);
   break;  

   case MD_Menu::DISP_CLEAR:
    lc.clearDisplay(0);
   break;

  case MD_Menu::DISP_L1:
    lc.setString(0,7, msg, 0);
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
      Serial.println("up");
      buttonPending = NO_BUTTONS_PENDING;
      return (MD_Menu::NAV_DEC);
    break;

    case PIN_BTN_DOWN:
      Serial.println("dn");
      buttonPending = NO_BUTTONS_PENDING;
      return (MD_Menu::NAV_INC);
    break;

    case PIN_BTN_SELECT:
      Serial.println("sel");
      buttonPending = NO_BUTTONS_PENDING;
      return (MD_Menu::NAV_SEL);
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
  timer1_write(rateDivisors[currentDivisor].cpuTicksPerFrame);
}

void setupLED() { 
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */   
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void setupButtonsandPins() {

  ButtonConfig* baseConfig = ButtonConfig::getSystemButtonConfig();
  baseConfig->setEventHandler(handleButtonEvent);
  baseConfig->setFeature(ButtonConfig::kFeatureClick);
  
  pinMode(PIN_BTN_SELECT, INPUT);
  buttonSelect.init(PIN_BTN_SELECT, HIGH, 0);
  
  pinMode(PIN_BTN_UP, INPUT);
  buttonUp.init(PIN_BTN_UP, HIGH, 1);

  pinMode(PIN_BTN_DOWN, INPUT);
  // since PIN_BTN_DOWN is zero, and the function is overloaded,
  // we need a cast to clear the compiler here. ugh.
  buttonDown.init((uint8_t)PIN_BTN_DOWN, HIGH, 2);

  pinMode(PIN_BTN_CLAPPER, INPUT);
  buttonClapper.init(PIN_BTN_CLAPPER, HIGH, 3);

  pinMode(LED_BUILTIN, OUTPUT);

  /* pins for timecode */
  pinMode(PIN_TC_IN, INPUT);
  pinMode(PIN_TC_OUT, OUTPUT);
}

void setup() {
  // Start serial
  Serial.begin(115200);
  delay(1000); // wait for serial to settle
  Serial.println("");

  initTimecode(&currentTime);

  // Load Configuration from EEPROM.
  loadConfig();
  
  /* LED Setup ---------------------------------- */
  setupLED();

  /* Master Clock ------------------------------- */
  setupTimer();

  /* Buttons and Blinky ------------------------- */
  setupButtonsandPins();

  /* Setup the interrupt on TC_IN for LTC reading */
  attachInterrupt(digitalPinToInterrupt(PIN_TC_IN), handleTCChange, CHANGE);

  /* menu */
  M.begin();
  M.setMenuWrap(true);
  M.setTimeout(MENU_TIMEOUT);
}

void showMenu() {
  lc.clearDisplay(0);
  lc.setString(0,7, "5ETUP", 0);
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

void loop() {
  // check all the buttons for activity.
  buttonUp.check();
  buttonDown.check();
  buttonSelect.check();
  buttonClapper.check();
  
  // Do we have valid timecode yet?
  if (bitRead(tcFlags, tcValid)) {
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
      Serial.println("\n\n\nSlate Started.");
      lc.setString(0,7,"FREECODE", 0);
      delay(1000);
      lc.clearDisplay(0);
      lc.setString(0,7,"Ver  1.0", 0);
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
      // when clapper closes do the thing.
      // freeze timecode for about four frames.
      delay(150);
      // show user bits
      showUserBits();
      delay(1000);
      lc.clearDisplay(0);
      delay(1000);
      state = STATE_TIMECODE;
      break;
    case STATE_TIMECODE:
      displayCurrentTime(&currentTime);
      break;
    case STATE_MENU:
      // handle timeouts. If menu times out we go back to TC
      if (M.isInMenu()) {
        M.runMenu();
      } else {
        state = STATE_TIMECODE;
      }
      break;
  }
  
}
