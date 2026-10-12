#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndPlayerStopSeq */

/* Stops the sequence on handle, fading out over fadeFrames frames. */
void Sound_StopHandle(SoundHandle* handle, int fadeFrames) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndPlayerStopSeq(handle, fadeFrames);
}
