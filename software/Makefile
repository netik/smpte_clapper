# Arduino CLI executable name and directory location
ARDUINO_CLI = arduino-cli

# Arduino CLI Board type
BOARD_TYPE ?= esp8266:esp8266:d1

# Default port to upload to
#SERIAL_PORT ?= /dev/tty.usbserial-230
SERIAL_PORT ?= /dev/cu.usbserial-2430

SKETCH=clapper_display

# Optional verbose compile/upload trigger
V ?= 0
VERBOSE=

# Build path -- used to store built binary and object files
BUILD_DIR=_build
BUILD_PATH=$(PWD)/$(BUILD_DIR)

ifneq ($(V), 0)
    VERBOSE=-v
endif

.PHONY: all example program clean

all: smpte_clapper

smpte_clapper: $(SKETCH)/$(SKETCH).ino
	$(ARDUINO_CLI) compile $(VERBOSE) --build-path='$(BUILD_PATH)' --build-cache-path='$(BUILD_PATH)' -b $(BOARD_TYPE) $(SKETCH)

upload: smpte_clapper
	$(ARDUINO_CLI) upload $(VERBOSE) -p $(SERIAL_PORT) --fqbn $(BOARD_TYPE) $(SKETCH)

install: upload

clean:
	rm -rf ./_build
	rm -rf $(BUILD_PATH)
	@rm *.elf
	@rm *.hex
