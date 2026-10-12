#include "types.h"
#include "game/Unk_022a8a10.h"

extern Unk_022a8a10 data_022a8a10;

void SND_StopTimer(int arg0, int arg1, u32 alarmMask, int arg3);
int SND_GetCurrentCommandTag(void);
void SND_WaitForCommandProc(int arg0);
void SND_FlushCommand(int arg0);

/* Stops the output (the first half of SoundOutput_Close). */
void SoundOutput_Pause(void) {
  Unk_022a8a10* state = &data_022a8a10;
  int saved;

  if (state->running == 0) {
    return;
  }
  SND_StopTimer(state->unk_20, state->unk_24,
                state->alarm >= 0 ? 1 << state->alarm : 0, 0);
  saved = SND_GetCurrentCommandTag();
  SND_FlushCommand(1);
  SND_WaitForCommandProc(saved);
}
