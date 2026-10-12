#include "types.h"
#include "game/sound.h"

/* Runs the NitroSystem sound update once the sound system is ready. */
void Sound_Update(void) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndMain();
}
