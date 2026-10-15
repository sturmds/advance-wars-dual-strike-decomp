#include "types.h"
#include "game/math.h"

u32 Rng_NextMixed(void);

/* Returns a random number from min to max, both included. */
s32 Rng_Range(s32 min, s32 max) {
  return min + HwMod32(Rng_NextMixed(), max - min + 1);
}
