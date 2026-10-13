#include "types.h"
#include "game/Unk_022a8a10.h"

BOOL func_0210928c(SoundFader* fader);
void func_021092a4(SoundFader* fader);
int SoundFader_GetValue(SoundFader* fader);
void func_0210fc88(int arg0, int volume, int arg2);
void SoundOutput_Close(void);

/* Per-frame step: advances the volume fader, closes the output when a
 * closing fade ends, otherwise sends the new volume if it changed. */
void SoundOutput_Update(void) {
  Unk_022a8a10* state = &data_022a8a10;
  SoundFader* fader;
  int volume;

  if (state->running == 0) {
    return;
  }
  if (state->unk_04 != 0) {
    return;
  }
  fader = &state->fader;
  func_021092a4(fader);
  if (state->closeWhenFaded != 0 && func_0210928c(fader)) {
    SoundOutput_Close();
    return;
  }
  volume = SoundFader_GetValue(fader) >> 8;
  if (volume != state->appliedVolume) {
    func_0210fc88(state->unk_20, volume, 0);
    state->appliedVolume = volume;
  }
}
