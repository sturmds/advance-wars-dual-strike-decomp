#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerMoveVolume */

/* Fades the music volume from startVolume to targetVolume over frames
 * frames. startVolume -1 starts from the player's set volume. */
void Sound_FadeMusicVolume(int startVolume, int targetVolume, int frames) {
  if (gSoundReady == 0) {
    return;
  }
  if (gMusicHandle.player == NULL) {
    return;
  }
  if (startVolume == -1) {
    startVolume = gMusicHandle.player->volume;
  }
  NNS_SndPlayerMoveVolume(&gMusicHandle, startVolume, 0);
  NNS_SndPlayerMoveVolume(&gMusicHandle, targetVolume, frames);
}
