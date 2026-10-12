#include "types.h"
#include "game/Unk_ov004_022bf158.h"

u64 OS_GetTick(void);
int Ov004_TicksToFrames(u64 ticks);

void Ov004_UpdateElapsedFrames(Unk_ov004_022bf158* state) {
  state->elapsedFrames = Ov004_TicksToFrames(OS_GetTick() - state->startTick);
}
