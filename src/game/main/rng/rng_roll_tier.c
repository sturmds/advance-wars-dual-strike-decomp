#include "types.h"

s32 HwMod32(s32 numer, s32 denom);
u32 Rng_Next(void);

/* Rolls 0 to 99 and returns 1, 2 or 3 for the first of the three cumulative
 * percent limits the roll is below, or 0 when it is below none. */
int Rng_RollTier(u32 limit1, u32 limit2, u32 limit3) {
  u32 roll = HwMod32(Rng_Next(), 100);

  if (roll < limit1) {
    return 1;
  }
  if (roll < limit2) {
    return 2;
  }
  if (roll < limit3) {
    return 3;
  }
  return 0;
}
