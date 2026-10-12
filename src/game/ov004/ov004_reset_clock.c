#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_ResetClock(Unk_ov004_022bf158* state) {
  int i;

  for (i = 0; i < OV004_TIMED_EVENT_COUNT; i++) {
    state->timedEventFired[i] = 0;
  }
  state->elapsedFrames = 0;
}
