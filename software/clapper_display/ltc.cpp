#include "ltc.h"
#include <string.h>
#include <Ticker.h>

const DIVISOR rateDivisors[] = {
  // rate  seconds per frame
  { 24,    0.041666667  }, 
  { 25,    0.04,        },
  { 29.97, 0.0333667    },
  { 30,    0.033333333   },
  { NULL,  NULL }
};

void initTimecode(TIMECODE *tc) {
  tc->hours = tc->minutes = tc->seconds = tc->frames = tc->micros = 0;
  memset(tc->userBits, 0, sizeof(tc->userBits));
}

float getDivisorForRate(float rate) {
  int x = 0;
  while (rateDivisors[x].frameRate != rate && rateDivisors[x].frameRate != NULL) { 
    x++;
  }

  if (rateDivisors[x].frameRate == NULL) { 
    return 0; 
  }

  return rateDivisors[x].secPerFrame;
}
