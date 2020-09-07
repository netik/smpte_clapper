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
 *   - Menu system 
 *      - Frame rate
 *      - Brightness
 */

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#include <Ticker.h>
#include "LedControl.h"
#include "slate.h"
#include "ltc.h"

#include <AceButton.h>
using namespace ace_button;

AceButton buttonA;
AceButton buttonSelect;
AceButton buttonClapper;

/* EEPROM config */
typedef struct configuration {
  // limit 512 bytes
  int displayBrightness;
  int frameRate;
} CONFIG;

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
    lc.clearDisplay(0);

    lc.setChar(0, 7, 'F', false);
    lc.setChar(0, 5, 'rRt', false);

    lc.setDigit(0,3,TENS(frameRate),false);
    lc.setDigit(0,2,ONES(frameRate),false);

    if (inDropMode) {
      lc.setChar(0,0,'d',false);
    }

}

void ledSetString(char *str) {
  char *x = str;
  int pos = 7;
  
  while (pos >= 0 && *x != '\0') {
     lc.setChar(0, pos, *x, false);
     pos--;
     x++;
   }

};

void handleButtonEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */)
{
  // generic button handler 
  switch (eventType) {
    case AceButton::kEventPressed:
    case AceButton::kEventRepeatPressed: {  
      uint8_t pin = button->getPin();
      switch    
    }
  }
}

void setup() {
  // start serial
  Serial.begin(115200);
  delay(1000); // wait for serial to settle
  Serial.println("");

  initTimecode(&currentTime);

  // Load Configuration from EEPROM.
  loadConfig();
  
  /* LED Setup ---------------------------------- */
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */   
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  /* Master Clock ------------------------------- */
  /* start the clock. we use hardware timer 1 and cannot use 0 -- 0 is used by WiFi */
  timer1_attachInterrupt(onTimerISR);

  /* Set up master timer
   *  The clock on the ESP8266 runs at 80Mhz (or 80 ticks/us).
   *  We need very precise timing for frames, so let's use the timer to set up
   *  a clock that exactly matches the frame rate.
   */
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);

  /*
   * Afaik the timers count down to zero and fire the interrupt at zero. 
   * timer1_write resets this countdown.
   */
  timer1_write(rateDivisors[currentDivisor].cpuTicksPerFrame);

  /* Buttons and Blinky ------------------------- */
  ButtonConfig* baseConfig = ButtonConfig::getSystemButtonConfig();
  baseConfig->setEventHandler(handleButtonEvent);
  baseConfig->setFeature(ButtonConfig::kFeatureClick);
  
  pinMode(PIN_SELECT, INPUT);
  buttonSelect.init(PIN_SELECT, HIGH, 0);
  
  pinMode(PIN_A, INPUT);
  buttonA.init(PIN_A, HIGH, 1);

  pinMode(PIN_CLAPPER, INPUT);
  buttonClapper.init(PIN_CLAPPER, HIGH, 2);

  pinMode(LED_BUILTIN, OUTPUT);

  /* pins for timecode */
  pinMode(PIN_TC_IN, INPUT);
  pinMode(PIN_TC_OUT, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_TC_IN), handleTCChange, CHANGE);
}

void loop() {
  // like a videogame we have a state machine.
  buttonA.check();
  buttonSelect.check();
  buttonClapper.check();
  
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
    userBits[10] = ((xtc[0] & 0xF0) >> 4) + '0';            // user bits 8
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

  // handle UI
  switch(state) {
    case STATE_HELLO:
      Serial.println("\n\n\nSlate Started.");
      showFrameRate();
      delay(1000);
      lc.clearDisplay(0);

      state = STATE_TIMECODE;
      Serial.println("Timecode Mode");
      break;
    case STATE_MENU:
      Serial.println("Menu Mode");
      break;
    case STATE_TIMECODE:
    default:
      displayCurrentTime(&currentTime);
  }
  
}
