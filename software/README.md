# freecode

This is an open source hardware and software solution that creates a 
SMPTE LED Time Code Slate, designed to emulate the very expensive 
commercial slates that exist. It's a hobbyist project for indie film
makers. These slates normally run for $1300 or more. This project can be
built for around $200. 

It is based around the ESP8266 and can generate and jam sync with other LTC devices.

Features
------------
* 7-segment SMPTE display
* LED menu system for easy configuration with three button control
* Magnetic hall-effect clapper
* LED to indicate jam sync
* Time Out: Display times out when sticks are left open. 
* Jam Lock: Inhibits running time code without jamming.
* Feed Alert: Reminds you when to jam. 
* Hold Clap Frame: Displays the last time code after the user bits.
* Flash Frame: Changes intensity to ensure exposure.  (on clap)
* Plus 1 FrameReader: Time code is displayed in real time when in read mode.
* Scroll back: Push UP button while sticks are closed to display
* Scroll back of previous claps.
* Scroll back memory is cleared at power off.
* Dumping of clap log over serial port (up to 99 claps)
* LTC Sync from Audio port (1.1v P-P Analog Audio Sync in, min 500mV p-p trigger)
* LTC Sync Audio Out (1.1v P-P)


TODO
------------

HW:
 * design output circuit - https://masteringelectronicsdesign.com/design-a-unipolar-to-bipolar-converter-for-a-unipolar-voltage-output-dac/
 * Battery voltage and low battery warning readout (can we read VCC?)
 * Fix Jam Sync at 24 and 25FPS. 29 and 30 work fine.

SW:
 * config: timeout -- depends on sticks being closed?
 	 * config: feed Alarm
 	 * config: plusOne reader
 * Generic numeric input system to support:
   * timecode: set user bits
   * timecode: set timeCode start time
 * SMPTE timecode generation
 * Frame Rate Error Alarms (how?!!)
 * auto frame lock - how?
 
Based on http://www.denecke.com/Support/Documents/TS-C_1013.pdf

Setting up the dev environment
------------

1. You need arduino-cli, so run this command (ew):
```
   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
```

2. add this to .cli-config.yml
```
  board_manager:
    additional_urls:
      - http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
4. Then do:
```
  arduino-cli core update-index
  arduino-cli core install esp8266:esp8266   
```
5. Install Libraries
```
  arduino-cli lib install MD_Menu
  arduino-cli lib install MD_UISwitch
  arduino-cli lib install LED_Control
  arduino-cli lib install ACEButton
```

6. You have to patch the LEDControl Library from Arduino. My copy has a lot more letters in it than the stock library, and is easier to read.

On Mac and Unix machines, The arduino data directory is
usually `~/Library/Arduino15` on OS X.

Copy over the LED Library:
```
cp -pR clapper_display/libraries/LedControl ~/Documents/Arduino/libraries/
```

Build
------------

Type "make" and "make upload" 


TOOLS
------------

You may also want to do:

     cp -pR tools ~/Documents/Arduino

to enable the ESPExceptionDecoder --

for usage, see:
https://github.com/me-no-dev/EspExceptionDecoder
