#include "types.h"

s32 HwMod32(s32 numer, s32 denom);
u32 func_02027a48(void);

/* Returns a random number from min to max, both included. */
s32 Rng_Range(s32 min, s32 max) {
    return min + HwMod32(func_02027a48(), max - min + 1);
}
