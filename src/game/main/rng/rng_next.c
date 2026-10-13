#include "types.h"
#include "game/rng.h"

/* Advances the RNG state and returns it: s = (4s + 2)(4s + 3) / 4, mod 2^32 before the shift. */
u32 Rng_Next(void) {
  u32 factor = gRngState * 4 + 2;
  gRngState = factor * (factor + 1) >> 2;
  return gRngState;
}
