#include "ltc.h"
#include <string.h>
#include <Ticker.h>

const DIVISOR rateDivisors[] = {
  // rate,  seconds per frame, cpu ticks/frame
  { 24,    0.041666667,  3333333 }, 
  { 25,    0.04,         3200000 },
  { 29.97, 0.0333667,    2669336 },
  { 30,    0.033333333,  2666667 },
  { NULL,  NULL, NULL }
};

void initTimecode(TIMECODE *tc) {
  tc->hours = tc->minutes = tc->seconds = tc->frames = 0;
    
  memset(tc->userBits, 0, sizeof(tc->userBits));
}

int getDivisorForRate(float rate) {
  int x = 0;
  while (rateDivisors[x].frameRate != rate && rateDivisors[x].frameRate != NULL) { 
    x++;
  }

  if (rateDivisors[x].frameRate == NULL) { 
    return 0; 
  }

  return x;
}
