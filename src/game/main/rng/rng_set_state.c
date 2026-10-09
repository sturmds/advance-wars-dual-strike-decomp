#include "types.h"

/* RNG state. */
extern u32 data_0216e2b4;

void Rng_SetState(u32 state) {
    data_0216e2b4 = state;
}
