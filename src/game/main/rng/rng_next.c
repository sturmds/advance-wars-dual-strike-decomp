#include "types.h"

/* RNG state. */
extern u32 data_0216e2b4;

/* Advances the RNG state and returns it: s = (4s + 2)(4s + 3) / 4, mod 2^32 before the shift. */
u32 Rng_Next(void) {
    u32 factor = data_0216e2b4 * 4 + 2;
    data_0216e2b4 = factor * (factor + 1) >> 2;
    return data_0216e2b4;
}
