#include "types.h"

u32 Rng_Next(void);
void Rng_SetState(u32 state);

/* Rng_Next, then adds bit 1 of the result into bit 0 and keeps that as the new state. */
u32 Rng_NextMixed(void) {
  u32 value = Rng_Next();
  value += (value & 2) >> 1;
  Rng_SetState(value);
  return value;
}
