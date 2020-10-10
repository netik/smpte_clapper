// config.h

/* EEPROM config */
typedef struct configuration {
  // limit 512 bytes
  int version;
  int displayBrightness;
  // framerate represented based on config options, not actual rate.
  int frameRate; 
  bool internal;
  int timeOut;
  int feed;
  bool jamLock;
  int flashHeld;
  int holdClap;
  bool backlight; // future
  bool plusOne;
  unsigned int magic; // used for checking if config is init'd
} CONFIG;
