#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerMoveVolume */

/* Fades the sound effect volume from startVolume to targetVolume over
 * frames frames. startVolume -1 starts from the player's set volume. */
void Sound_FadeSeVolume(int startVolume, int targetVolume, int frames) {
  if (gSoundReady == 0) {
    return;
  }
  if (gSeHandle.player == NULL) {
    return;
  }
  if (startVolume == -1) {
    startVolume = gSeHandle.player->volume;
  }
  NNS_SndPlayerMoveVolume(&gSeHandle, startVolume, 0);
  NNS_SndPlayerMoveVolume(&gSeHandle, targetVolume, frames);
}
