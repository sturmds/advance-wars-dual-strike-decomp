#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerStopSeq */

/* Stops the music with a one-frame fade. */
void Sound_StopMusic(void) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndPlayerStopSeq(&gMusicHandle, 1);
}
