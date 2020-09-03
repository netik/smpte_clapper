/* Useful Constants - from DateTime.h */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time - requres input as number of seconds */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

/* requires input as number of miliseconds */
#define numberOfMs(_time_) (_time_ - 1000 * floor(_time_ / 1000))

/* System States */
#define STATE_HELLO     0
#define STATE_TIMECODE  1
#define STATE_MENU      2
#define STATE_FRSELECT  3

/* return the place value for a number */
#define TENS(n) (floor((n) / 10))
#define ONES(n) ((n) - (floor((n) / 10)) * 10)

/* how often we hit the interrupt for the timer */
#define TIMER_TICKS_INTR 5000

/* ESP8266 pin notes

   D0 = GPIO16 (WAKE), pin 4, Data in, no interrupt, no pwm, no i2c
   D1 = GPIO5 (i2c SCL), pin 20, CS, ok
   D2 = GPIO4 (i2c SDA), pin 19, Clock, ok
   D3 = GPIO0 (FLASH), pin ?, pulled up in, ok out, by deafult, boot fails if pulled low
   D4 = GPIO2 (-), pin ?, pulled up in, ok out, high at boot, onboard led, boot fails if pulled low
   D5 = GPIO14 (SPI SCLK), pin ?, in/out ok
   D6 = GPIO12 (SPI MISO), pin ?, in/out ok
   D7 = GPIO13 (SPI MOSI), pin ?, in/out ok
   D8 = GPIO15 (SPI CS), pin ?, in/out ok
   RX = GPIO3 (out) high at boot
   TX = GPIO1 (in) high at boot
   A0 = Analog Input

   RTS and DTR from U2 seem to go into GPIO0, which means it's probably not safe to use.
   (no, turns out that's fine.)
*/
/* Current wiring is:

   ~SELECT (D8) does not need pulldown
   ~UP (D7) needs pull down
   ~DOWN (D6) needs pull down

   Timecode In (D5)
   Timecode Out (D4)
   Clapper Trigger (D3)

   We also need -

   CLK (red) D2 / GPIO4
   CS (green) D1 / GPIO5
   DIN (yellow) D0 / GPIO16

   Pin reference: Note that all pins are GPIO pins, not physical pins.
   LedControl(data, clock, load/cs, number of MAX 7219 devices?)
*/

// from https://github.com/esp8266/Arduino/blob/master/variants/d1_mini/pins_arduino.h
// is apparently gpio mapping to d# notation
#define PIN_G_D0 16
#define PIN_G_D1 5
#define PIN_G_D2 4
#define PIN_G_D3 0
#define PIN_G_D4 2
#define PIN_G_D5 14
#define PIN_G_D6 12
#define PIN_G_D7 13
#define PIN_G_D8 15

// this maps in to physical pin
#define PIN_D0 4  // 4/io16/wake/nodemcu0/arduino16 (works on 16)
#define PIN_D1 20 // 20/io5/control1/arduino5 (works on 5) 
#define PIN_D2 19 // 19/io4/nodemucu2/arduino4 (works on 4)
#define PIN_D3 18 // 18/io0/cs2/nodemcu3/arduino0 (high run low flash)
#define PIN_D4 17 // 17/io2/txd1/nodemcu4/arduino2
#define PIN_D5 5  // 5/io14/hspclk/nodemcu5/clk/arduino14
#define PIN_D6 6  // 6/io12/hspiq/nodemcu6/miso/arduino12
#define PIN_D7 7  // 7/io13/hspid/cts0/rxd2/7/mosi/arduino13
#define PIN_D8 16 // 16/io15/hspics/rts0/txd2/nodemcu8/cs/15

/* Buttons */
/* see also https://i.imgur.com/Tno6Eib.jpg
 *  
 *   Pin Function       ESP-8266 Pin
 *   A0 DAC             Analog input, max 3.3V input A0
 *   TX TXD             TXD
 *   RX RXD             RXD
 *   D0 IO              GPIO16
 *   D1 IO, SCL         GPIO5
 *   D2 IO, SDA         GPIO4
 *   D3 IO,10k Pull-up  GPIO0
 *   D4 IO, 10k pull-up, BUILTIN_LED GPIO2
 *   D5 IO, SCK         GPIO14
 *   D6 IO, MISO        GPIO12
 *   D7 IO, MOSI        GPIO13
 *   D8 IO,10k pull-down, SS GPIO15
 *   G Ground           GND
 *   5V 5V              -
 *   3V3 3.3V           3.3V
 *   RST Reset          RST
 */
 
/**
 * GPIO0: pull low during boot for flash mode (connects to a push button).
 *        Other than that, usable as GPIO but beware of waht you connect to it.
 * GPIO1, 3: TX and RX (Serial). Also usable as GPIO if not using serial.
 * GPIO2: is pulled up during boot & internal LED (active LOW).
 * GPIO15: fixed external pull-down (for boot).
 * GPIO4, 5, 12-14, 16: nothing special.
 */

/* USE GPIO NUMBERS ONLY HERE */

/* GPIO Pins -------------------------------------------- */
/* display */
#define PIN_DISP_CLK     PIN_G_D2 // test ok OUTPUT
#define PIN_DISP_CS      PIN_G_D1 // test ok OUTPUT
#define PIN_DISP_DIN     PIN_G_D0 // test ok OUTPUT

/* buttons */
#define PIN_CLAPPER      PIN_G_D6 // tbd, requires pulldown

#define PIN_A            PIN_G_D7 // test ok INPUT. switch should be tied to +3.3v
#define PIN_SELECT       PIN_G_D8 // test ok INPUT w/pulldown, switch should be tied to +3.3v

#define PIN_TC_IN        A0       // solves our adc issue I guess.
#define PIN_TC_OUT       PIN_G_D5 // gpio 14.

/* end GPIO Pins ---------------------------------------- */

#define NUM_BUTTONS      2
