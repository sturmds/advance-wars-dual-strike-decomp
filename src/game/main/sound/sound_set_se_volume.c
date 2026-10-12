#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerSetVolume */

void Sound_SetSeVolume(int volume) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndPlayerSetVolume(&gSeHandle, volume);
}
