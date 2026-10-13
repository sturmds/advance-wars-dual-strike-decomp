#include "types.h"
#include "game/Unk_022a8a10.h"
#include "sdk/os.h"

void MIi_CpuClear32(u32 value, void* dst, u32 size);
void SND_StartTimer(int arg0, int arg1, u32 alarmMask, int arg3);

/* Clears both output buffers and starts the output again. The volatile zeros
 * are needed to match: the original passes the fill value from the stack. */
void SoundOutput_Restart(void) {
  Unk_022a8a10* state = &data_022a8a10;
  volatile u32 zeroL;
  volatile u32 zeroR;

  if (state->running == 0) {
    return;
  }
  state->unk_18 = 0;
  zeroL = 0;
  MIi_CpuClear32(zeroL, state->bufferL, state->bufferSize);
  zeroR = 0;
  MIi_CpuClear32(zeroR, state->bufferR, state->bufferSize);
  DC_FlushRange(state->bufferL, state->bufferSize);
  DC_FlushRange(state->bufferR, state->bufferSize);
  SND_StartTimer(state->unk_20, state->unk_24,
                 state->alarm >= 0 ? 1 << state->alarm : 0, 0);
}
