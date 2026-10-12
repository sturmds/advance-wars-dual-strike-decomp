#include "types.h"
#include "game/sound_handle.h"

int SoundFader_GetValue(SoundFader* fader);

/* Starts a fade from the fader's current value to target over frames. */
void SoundFader_Start(SoundFader* fader, int target, int frames) {
  fader->start = SoundFader_GetValue(fader);
  fader->target = target;
  fader->frames = frames;
  fader->elapsed = 0;
}
