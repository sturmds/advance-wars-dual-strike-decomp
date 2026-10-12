#include "types.h"

/* RNG state. */
extern u32 gRngState;

void Rng_SetState(u32 state) { gRngState = state; }
