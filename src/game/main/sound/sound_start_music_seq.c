#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* NitroSystem NNS_SndArcPlayerStartSeq: returns TRUE if the sequence started. */

/* Starts sound archive sequence seqNo on the music handle. */
void Sound_StartMusicSeq(int seqNo) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndArcPlayerStartSeq(&gMusicHandle, seqNo);
}
