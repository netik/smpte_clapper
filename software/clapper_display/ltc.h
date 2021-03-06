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
  char name[7];
  float frameRate;                // displayed frame rate
  bool canDropFrames;             // if user can select drop frame mode

  float secPerFrame;              // seconds per frame as float
  uint32_t cpuTicksPerFrame;      // cpu ticks (1uS) per frame
  uint32_t cpuTicksPerBit;        // cpu ticks (1uS) per bit

  /* edge change differences */
  int uMax0;                      // the max timing between edge changes - Any time greater than this is to big
  int uMax1;                      // the midpoint between a zero and a one
  int uMin1;                      // anything less than this time is too short.
} DIVISOR;

/* prototypes */
extern void initTimecode(TIMECODE *);
extern DIVISOR *getDivisorForRate(float);
extern DIVISOR rateDivisors[];

#endif // __LTC_H__