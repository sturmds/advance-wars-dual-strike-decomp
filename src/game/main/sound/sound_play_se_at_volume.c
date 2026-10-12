#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

void Sound_PlaySe(u32 soundId);
/* NitroSystem NNS_SndPlayerSetVolume */

/* Plays sound effect soundId at the given player volume. */
void Sound_PlaySeAtVolume(u32 soundId, int volume) {
  if (gSoundReady == 0) {
    return;
  }
  Sound_PlaySe(soundId);
  NNS_SndPlayerSetVolume(&gSeHandle, volume);
}
