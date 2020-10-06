freecode
=====

This is an open source hardware and software solution that creates a 
SMPTE LED Time Code Slate, designed to emulate the very expensive 
commercial slates that exist.

It is based around the ESP8266 and can generate and jam sync with other LTC devices.

Features
------------
* 7-segment menu system for easy configuration
* LED to indicate jam sync
* Three button menu systetm
* Time Out: Display times out when sticks are left open. 
* EL Backlight: Enable/disable in low brightness.
* Jam Lock: Inhibits running time code without jamming.
* Feed Alert: Reminds you when to jam. 
* Hold Clap Frame: Displays the last time code after the user bits.
* Flash Frame: Changes intensity to ensure exposure.  (on clap)
* Plus 1 FrameReader: Time code is displayed in real time when in read mode.
* Scroll back: Push UP button while sticks are closed to display
* Scroll back of previous claps.
* Scroll back memory is cleared at power off.

TODO
=====

Remaining:

 * test timecode locking and maybe build better input circuit
 * design output circuit - https://masteringelectronicsdesign.com/design-a-unipolar-to-bipolar-converter-for-a-unipolar-voltage-output-dac/
 * config: timeout -- depends on sticks being closed?
 	* config: feed Alarm
 	* config: plusOne reader
 * timecode: set user bits
 * timecode: set timecode time like a clock
 * SMPTE timecode generation
 	 * should be switched and independent of jam sync.
 * Frame Rate Error Alarms (how?!!)
 * auto frame lock - how?
 
 * http://www.denecke.com/Support/Documents/TS-C_1013.pdf
 * Battery voltage and low battery warning readout

Set up
==============
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
======================================================

Type "make".


TOOLS
=================

You may also want to do:

     cp -pR tools ~/Documents/Arduino

to enable the ESPExceptionDecoder --

for usage, see:
https://github.com/me-no-dev/EspExceptionDecoder
