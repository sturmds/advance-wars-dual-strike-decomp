#include "types.h"

extern s8 gSndSineQuarterTable[];

/* One period is 128 steps; the table holds the first quarter plus the peak. */
s8 SndDrv_SineWave(int phase) {
  if (phase < 32) {
    return gSndSineQuarterTable[phase];
  }
  if (phase < 64) {
    return gSndSineQuarterTable[64 - phase];
  }
  if (phase < 96) {
    return -gSndSineQuarterTable[phase - 64];
  }
  return -gSndSineQuarterTable[32 - (phase - 96)];
}
