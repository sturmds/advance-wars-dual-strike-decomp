#include "types.h"
#include "game/Unk_ov004_022bf158.h"

u64 func_0210e8b4(void);

void Ov004_StartClock(Unk_ov004_022bf158 *state) {
    state->startTick = func_0210e8b4();
}
