
Install Libraries

You need:


Build


1. You need arduino-cli, so run this command (ew):

   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

2. add this to .cli-config.yml

  board_manager:
    additional_urls:
      - http://arduino.esp8266.com/stable/package_esp8266com_index.json

3. Set the board type, For the 8266 wemos d1, it's:
    
esp8266:esp8266:d1

4. Type make

