Set up
======================================================
1. You need arduino-cli, so run this command (ew):

   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

2. add this to .cli-config.yml

  board_manager:
    additional_urls:
      - http://arduino.esp8266.com/stable/package_esp8266com_index.json

4. Then do:

  arduino-cli core update-index
  arduino-cli core install esp8266:esp8266   

5. Install Libraries

  arduino-cli lib install MD_Menu
  arduino-cli lib install MD_UISwitch
  arduino-cli lib install LED_Control
  arduino-cli lib install ACEButton


6. You have to patch the LEDControl Library from Arduino.

On Mac and Unix machines, The arduino data directory is
usually ~/Library/Arduino15. Copy over the LED Library:

cp -pR clapper_display/libraries/LedControl ~/Documents/Arduino/libraries/

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
