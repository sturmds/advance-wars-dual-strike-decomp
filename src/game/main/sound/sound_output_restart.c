#include "types.h"
#include "game/Unk_022a8a10.h"

extern Unk_022a8a10 data_022a8a10;

void DC_FlushRange(void *addr, u32 size);
void func_0210f474(u32 value, void *dst, u32 size);
void func_0210fdc8(int arg0, int arg1, u32 alarmMask, int arg3);

/* Clears both output buffers and starts the output again. The volatile zeros
 * are needed to match: the original passes the fill value from the stack. */
void SoundOutput_Restart(void)
{
    Unk_022a8a10 *state = &data_022a8a10;
    volatile u32 zeroL;
    volatile u32 zeroR;

    if (state->running == 0) {
        return;
    }
    state->unk_18 = 0;
    zeroL = 0;
    func_0210f474(zeroL, state->bufferL, state->bufferSize);
    zeroR = 0;
    func_0210f474(zeroR, state->bufferR, state->bufferSize);
    DC_FlushRange(state->bufferL, state->bufferSize);
    DC_FlushRange(state->bufferR, state->bufferSize);
    func_0210fdc8(state->unk_20, state->unk_24, state->alarm >= 0 ? 1 << state->alarm : 0, 0);
}
