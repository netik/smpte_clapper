#include "ltc.h"
#include <string.h>
#include <Ticker.h>

/*
  29.97 vs. Drop Frame

  Many people confuse 29.97 with drop-frame but they are not the
  same thing: 29.97 indicates a rate (frequency) and drop-frame
  indicates a format (count).
  
  “29.97” time code is 30 fr/sec code with a rate of 29.97. When
  generating 29.97 time code, the generator locks to a field rate
  of 59.94 Hz (NTSC color rate) and pulls down the frame rate by
  .1% to 29.97. But the format or frame count remains 30 fr/sec.
  Drop Frame on the other hand is another way of counting
  frames. 

  It is a variation of 30 fr/sec format, invented to compensate 
  for the color rate of 29.97. Drop frame code leaves certain
  frame numbers out so that one hour of code time equals one
  hour of stopwatch time when code is running at 29.97.
  Drop and non-drop code can both be generated at a rate of 30
  or 29.97 fr/sec. For that reason, the frame rate and the drop
  frame status are displayed separately.
*/
DIVISOR rateDivisors[] = {
  // I didn't write this decoder, but... 
  //
  //    umax0 is the maximum length of a bit
  //    umin1 is the minimum length of a transition and any timing < umin1
  //          will be discarded.
  //    umax1 is a half bit length. Two half bit transitions == We recv'd a 1

  // the code and circuit has about a 60-100uS delay, so you should add
  // that time onto the umax values here. e.g. umax0 here is set to
  // 521uS for 30fps whereas 30fps is actually one full bit every 416.67uS
  // and a half bit every 208.35uS. You also want some slop in here (5% or so)
  // in case the transmitter isn't dead-on.

  // The most important value is the length of a zero bit as everything is keyed
  // off of it.

  // tested rates: 24,29,29d,30 ok.

  // name,  rate,   drop,  sec/frame,    tick/fr, tick/bit, umax0, umax1, umin1 
  { "23",  23.976,  false, 0.041708375,  3336670, 417084,   626,   313,   104 },
  { "24",  24,      false, 0.041666667,  3333333, 416667,   626,   313,   104 },
  { "25",  25,      false, 0.04,         3200000, 400000,   600,   300,   104 },
  { "29",  29,      false, 0.0333667,    2669336, 333667,   521,   261,   104 },
  { "29D", 29,      true,  0.0333667,    2669336, 333667,   521,   261,   104 },
  { "30",  30,      false, 0.033333333,  2666667, 333333,   521,   261,   104 },
  { NULL,  NULL,   NULL,         NULL,    NULL }
};

void initTimecode(TIMECODE *tc) {
  tc->hours = tc->minutes = tc->seconds = tc->frames = 0;
    
  memset(tc->userBits, 0, sizeof(tc->userBits));
}
 
DIVISOR *getDivisorForRate(float rate) {
  int x = 0;
  while (rateDivisors[x].frameRate != rate && rateDivisors[x].frameRate != NULL) { 
    x++;
  }

  if (rateDivisors[x].frameRate == NULL) { 
    // end reached
    return NULL;
  }

  return &(rateDivisors[x]);
}

// todo - timecode math lib?
// https://www.npmjs.com/package/smpte-timecode
