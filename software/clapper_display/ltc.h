/**
 * ltc.h
 *
 * defines and utility header for smpte linear time code decoding
 * 
 */

/**
 * 
 * SMPTE Time standard
 * https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=7291029
 * 
 * Another take on the standard:
 * https://www.connect.ecuad.ca/~mrose/pdf_documents/timecode.pdf
 * 
 */

#ifndef __LTC_H__
#define __LTC_H__
#include <Ticker.h>

/* jna - NTSC 29.97, but seems to work for 30?  */
#define uMax0 521 // Any time greater than this is to big
#define uMax1 261 // Midpoint timing between a 1 and 0 bit length
#define uMin1 104 // Any time less than this is to short

typedef struct timecode {
   int hours;
   int minutes;
   int seconds;
   int frames;
   char userBits[12];
} TIMECODE;


enum flagBits {
  tcValid,        // TC copied to xtc is valid (Used by main loop to determing if a timecode has arrived)
  tcFrameError,   // ISR edge out of timing bounds (only gets reset after next valid TC read)
  tcOverrun,      // TC was not read from xtc by main loop before next value was ready (so main loop can tell if timecodes have been lost)
  tcForceUpdate,  // Valid TC will always be copied to buffer even if last value not read (if set by main code then TC will always be copied)
  tcHalfOne       // ISR is reading a 1 bit so ignore next edge (Internal to ISR)
};

/* frame divisors */
typedef struct divisor {
  float frameRate;
  float secPerFrame;
  unsigned long cpuTicksPerFrame;
} DIVISOR;

/* prototypes */
extern void initTimecode(TIMECODE *);
extern int getDivisorForRate(float);
extern const DIVISOR rateDivisors[];

#endif // __LTC_H__