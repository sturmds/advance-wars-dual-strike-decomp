#include "types.h"

extern u32 gOv003RandState;

/* Steps the overlay 3 random state. func_ov003_02351814 sets the seed. */
u32 Ov003_RandNext(void)
{
    u32 state = gOv003RandState * 4 + 2;

    state = (state * (state + 1)) >> 2;
    state = state + ((state & 6) >> 1);
    gOv003RandState = state;
    return state;
}
