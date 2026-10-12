#include "types.h"

extern u32 gOv003RandState;

/* Steps the overlay 3 random state. Ov003_RandSeed sets the seed. */
u32 Ov003_RandNext(void) {
  u32 state = gOv003RandState * 4 + 2;

  state = (state * (state + 1)) >> 2;
  state = state + ((state & 6) >> 1);
  gOv003RandState = state;
  return state;
}
