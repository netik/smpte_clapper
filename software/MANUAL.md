# Freecode User Manual

## What is this?

Freecode is an open source *digital timecode slate* based on the ESP8266. It is designed to emulate expensive Hollywood style slates. It consists of a schematic, printed circuit board design, and firmware, written in Arduino's *sketch* format and C/C++. 

This is the user manual. For implementation details, see README.MD, the code, and the schematics.

## Startup

On reset, The system will display "freecode" followed by the build number (bld 1.0), the current FPS rate, and timecode will start.

If the internal generator is active, the LED will blink at a rate of one blink per frame and will stay on for the first five frames of each second.

## Display

The display is a seven segment display with decimal points. During normal operation the display shows Hours, minutes, seconds, and frames. 

In history mode, the very last decimal point is lit to indicate that you are not looking at real-time time code. 

## Buttons

There are four buttons on the device: **UP**, **DOWN**, **SELECT**, and **RESET**.

**UP** **DOWN** and **SELECT** can be used to walk through menus and select options.

**RESET** is hard reset, resetting the microcontroller. In normal operation, you should never have to press the **RESET** button. 

Additionally a hall-effect switch and magnet will be located at the top is used to detect the opening and closing of the sticks, This is the CLAP switch.

## What happens on a "clap" ?

When the sticks are closed initally, no timecode is displayed to save power. When the sticks are opened, the current timecode is displayed until the sticks are closed again or the *Tout* timer is exhausted.

If the sticks are closed after being opened, the system will register a *clap*.

During a **clap**, the following happens:

1. The current timecode is copied to the last position in the timecode history.
2. **If *flash* is greater than zero**, the timecode is shown at maximum intensity on the display for the desired number of frames. Otherwise, the clapped time code is displayed at normal intensity for three frames.
3. The user bits are shown on the display. If no userbits have been recieved or set, the system will display "**00c0deXX**" where **XX** is the current **frames per second**. (User bits are shown with all decimal points turned on)
4. The display pauses for one second.
5. **If *hold* is greater than zero**, the clapped timecode is held on the screen for the requested number of seconds.
6. The display is turned off. 

Note that you cannot clap again until this sequence of events completes.

## Menu

To enter the menu, press **SELECT** from **TIMECODE MODE**. 

From there, you can use **UP** and **DOWN** to walk through the menus. **SELECT** will select a menu option for editing.

You'll know you're in edit mode when the = (equals) sign appears in the display.

**UP** and **DOWN** can be used to change values once a menu option has been seleted. 

To register the selection hit **SELECT** again.

**Note that the slate's configuration is not saved until STORE is selected from the menu.**

Configuration options are temporary until you **STORE** them.

### Menu options

| Option | Possible Values | Default on init | Use |
|--------|--------|---------|-----|
|brite   | 1 - 7  | 2       | Display Brightness |
|IntG    | yes / no  | yes       | Internal generator on/off. If this is off and no valid timecode is provded on the timecode input, the clock will not advance. |
|frt     | 23, 24, 25, 29, 29d, 30 | 30 non-drop | Frame Rate. 29 is 29.97, 29d is 29.97 drop|
|Feed    | 0, 2, 4, 6, 8 |8| Feed timeout. When these many hours have elapsed, timecode will stop and the slate will demand to be jam-sync'd again. 0 disabled. Ignored unless jloc is set to true. |
|Tout    | 0, 15, 30, 60, 120 | 60 | Open Sticks Timeout (in seconds) |
|jloc    | yes / no | no | Stops timecode from running unless the slate has been jam-sync'd at least once. |
|Flash   | 0 - 5 | 3 | When the slate closes (a "clap"), the current timecode will be displayed for these many frames and displayed at maximum brightness. 0 disables. |
|Hold    | 0, 5, 15, 30, 60, 120 | 0 | How many seconds to display the clapped timecode for, after the flash and user bits are displayed. 0 disables. |
|Plus1   | yes / no |no| Adds one to the displayed time code so that the timecode is in "real time". Normally the displayed timecode is always one frame behind. |
|init    | -- | -- | Selecting this, and then confirming by selecting "y" will reset the configuration to factory defaults. |
|store   | -- | -- |Selecting this option will save the configuration. The system will display "Stored." and then you will be returned to TIMECODE MODE |
|return  | -- | -- |Selecting this menu option will return to TIMECODE MODE. A quick way out of the menu system is to press UP and select after hitting select the first time. |

The menu option will roll over automatically. (i.e. from "return" the next option is "Bright.", and from "bright" the previous option is "return".)

If you do nothing, the menu will time out in 5 seconds.

Change `#define MENU_TIMEOUT 5000` in menu.h, and recompile to increase this timeout.

Note that if your slate has been jam-sync'd to an external clock you should jam sync again after saving a configuration change. The microcontroller's interrupt timer has to be disabled to write to the EEPROM and when that happens, the jam'd clock will drift.

### History Mode

The system retains the last 16 clap events.

To see previous claps, you can press UP while in timecode mode. The system will display "-----16" showing the last clap. The oldest clap is in position 1, the last in position 16.

Connecting a USB cable to the device and hitting 


UP and DOWN will move through these claps, and SELECT will exit this mode.

The clap history is lost on reset. A future code revision will store this data in the eeprom.


