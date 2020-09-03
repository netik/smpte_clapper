#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif
#include "LedControl.h"
#include <Ticker.h>
#include "slate.h"

#include <AceButton.h>
using namespace ace_button;

AceButton buttonA;
AceButton buttonSelect;
AceButton buttonClapper;

/**
 * freeslate
 * J. Adams <jna@retina.net> 
 * 9/2020
 * 
 * An open source SMPTE clapper board, similar to commercial solutions.
 * Reads, writes, and displays SMPTE time code.
 * 
 * TODO:
 *   - SMPTE timecode jamming
 *   - SMPTE timecode generation
 *   - Menu system to select frame rate
 *   - Brightness control over display
 */

/* EEPROM config */
typedef struct configuration {
  // limit 512 bytes
  int displayBrightness;
  int frameRate;
} CONFIG;

/* frame divisors */
typedef struct divisor {
  float frameRate;
  float msPerFrame;
} DIVISOR;

// On the ESP8266, note that these pins are the GPIO number not the physical pin and that
// even though the device is a 5V device logic works fantastically fine on 3.3v!
LedControl lc=LedControl(PIN_DISP_DIN, PIN_DISP_CLK, PIN_DISP_CS, 1);

/**
 * 
 * SMPTE Time standard
 * https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7291029
 * 
 * Another take on the standard:
 * https://www.connect.ecuad.ca/~mrose/pdf_documents/timecode.pdf
 * 
 */
const DIVISOR rateDivisors[] = {
  { 24, 0.024 }, 
  { 25, 0.025 },
  { 29.97, 0.02997 },
  { 30, 0.03 }
};

/* Globals - System State */
unsigned long currentTime;
float frameDivisor = 0.03; // frame length in mS
bool  inDropMode = false;
float frameRate = 30;

/* what are we doing? */
int state = STATE_HELLO;

/** TBD
 *  load and process configuration from EEPROM
 */
CONFIG *loadConfig() {
  return false;
};


/**
 * So what I think we need to do is to maintain a frame number counter instead of a 
 * millis counter, and we increment that every second or so, but in the case of a drop
 * frame, we have to do:
 * 
 * Because 10 minutes is not evenly divisible by 18 frames, we use drop-frame timecode
 * and drop two frame numbers every minute; by the ninth minute, you have dropped all
 * 18 frame numbers. No frames need to be dropped the tenth
 * minute. That is how drop-frame timecode works. 
 * When you use drop-frame timecode, Premiere 5.x renumbers the
 * first two frames of every minute, except for every tenth minute.
 * 
 * Timecode measures time in Hours:Minutes:Seconds:Fractions-of-seconds called frames. 
 * However, in NTSC video, a frame is not an even fraction of a second! 
 * Thus, NTSC timecode is always subtly off from real time—by exactly 1.8 frames per minute. 
 * Drop-frame timecode numbering attempts to adjust for this discrepancy by dropping two 
 * numbers in the numbering sequence, once every minute except for every tenth minute (see the preceding section,
 * Mathematics of 29.97 video, for details).The numbers that are dropped are 
 * frames 00 and 01 of each minute; thus, drop-frame numbering across the 
 * minute boundary looks like this: 
 * ... 00:00:59:27, 00:00:59:28, 00:00:59:29, 00:01:00:02, 00:01:00:03, ...
 *
 * Note, however, that you are off by only 1.8 frames per minute. If you adjust by two 
 * full frames every minute, you are still off by a little. Let's go through a sequence 
 * of minutes, to see how far off we are each minute, and where each adjustment leaves us:
 * (see the graph)
 * 
 * On the 10 minute, you drop zero. 
 */

/* ISR for timer tick */
void ICACHE_RAM_ATTR onTimerISR(){
  currentTime++;
}


/**
 * show the current smpte time on the display.
 * @params {Integer} val number of milliseconds
 **/ 
void displayCurrentTime(unsigned long val) {
    // setDigit: addr, digit, value, decimal point.
    // Digit 7 is far left.
    int hours = numberOfHours(val / 1000);
    int minutes = numberOfMinutes(val / 1000);
    int seconds = numberOfSeconds(val / 1000);
    int frames = numberOfMs(val) * frameDivisor;
    
    // Hours
    lc.setDigit(0,7,TENS(hours),false);
    lc.setDigit(0,6,ONES(hours),true);

    // Minutes    
    lc.setDigit(0,5,TENS(minutes),false);
    lc.setDigit(0,4,ONES(minutes),true);

    // seconds
    lc.setDigit(0,3,TENS(seconds),false);
    lc.setDigit(0,2,ONES(seconds),true); // seems to set decimal point fine. 

    // frames
    lc.setDigit(0,1,TENS(frames),false);
    lc.setDigit(0,0,ONES(frames),false);

    // Wink the LED once a second. (it appears to be active low?)
    // We try hard here not to write if we don't have to.
    if (numberOfMs(val) < 500) { 
      digitalWrite(LED_BUILTIN, false);
    } else {
      digitalWrite(LED_BUILTIN, true);
    }
}

void showFrameRate() {
    lc.clearDisplay(0);

    lc.setChar(0, 7, 'F', false);
    lc.setChar(0, 5, '-', false);

    lc.setDigit(0,3,TENS(frameRate),false);
    lc.setDigit(0,2,ONES(frameRate),false);

    if (inDropMode) {
      lc.setChar(0,0,'d',false);
    }

    delay(2000);
    lc.clearDisplay(0);
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

void handleButtonEvent(AceButton* button, uint8_t eventType,
    uint8_t /* buttonState */)
{
  // generic button handler 
  switch (eventType) {
    case AceButton::kEventPressed:
    case AceButton::kEventRepeatPressed: {  
      uint8_t pin = button->getPin();
      Serial.println(pin);      
    }
  }
}

void setup() {
  // start serial
  Serial.begin(115200);
  delay(1000); // wait for serial to settle
  Serial.println("");
    
  // Load Configuration from EEPROM.
  loadConfig();
  currentTime = millis();
  
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
  /* start the clock. we use hardware timer 1 instead of 0; 0 is used by WiFi */
  timer1_attachInterrupt(onTimerISR);

  /* Set up master timer
   *  The clock on the ESP8266 runs at 80Mhz (or 80 ticks/us).
   *  We run the divisor at TIM_DIV16 continuously to drive the master clock.  
   *  5000 ticks = 100mS (80000000/16/5000)
   */
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(TIMER_TICKS_INTR);

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
}

void loop() {
  // like a videogame we have a state machine.
  buttonA.check();
  buttonSelect.check();
  buttonClapper.check();
  
  switch(state) {
    case STATE_HELLO:
      Serial.println("\n\n\nSlate Started.");
      ledSetString("HELL0");
      delay(1000);
      showFrameRate();
      state = STATE_TIMECODE;
      Serial.println("Timecode Mode");
      break;
    case STATE_MENU:
      //Serial.println("Menu Mode");
      ledSetString("F 30");
      break;
    case STATE_TIMECODE:
    default:
      displayCurrentTime(currentTime);
  }
  
  delay(10);    // maybe not even needed.
}
