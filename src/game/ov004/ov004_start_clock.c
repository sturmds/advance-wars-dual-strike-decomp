#include "types.h"
#include "game/Unk_ov004_022bf158.h"

u64 OS_GetTick(void);

void Ov004_StartClock(Unk_ov004_022bf158* state) {
  state->startTick = OS_GetTick();
}
