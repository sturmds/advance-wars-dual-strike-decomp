#include "types.h"
#include "game/ov003_random.h"
#include "game/ov003_unit.h"

u32 gOv003RandState;
Ov003Input data_ov003_02365b04;

/* Steps the overlay 3 random state. Ov003_RandSeed sets the seed. */
u32 Ov003_RandNext(void) {
  u32 state = gOv003RandState * 4 + 2;

  state = (state * (state + 1)) >> 2;
  state = state + ((state & 6) >> 1);
  gOv003RandState = state;
  return state;
}
