#include "types.h"

/* RNG state. */
extern u32 gRngState;

/* Advances the RNG state and returns it: s = (4s + 2)(4s + 3) / 4, mod 2^32 before the shift. */
u32 Rng_Next(void) {
  u32 factor = gRngState * 4 + 2;
  gRngState = factor * (factor + 1) >> 2;
  return gRngState;
}
