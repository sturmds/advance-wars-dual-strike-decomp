#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Returns 1 the first time the clock has reached `frame` for this event. */
int Ov004_CheckTimedEvent(Unk_ov004_022bf158* state, int event, int frame) {
  int elapsed = state->elapsedFrames;

  if (state->timedEventFired[event] != 0) {
    return 0;
  }
  if (elapsed < frame) {
    return 0;
  }
  state->timedEventFired[event] = 1;
  return 1;
}
