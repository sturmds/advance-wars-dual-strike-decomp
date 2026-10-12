#include "types.h"
#include "game/sound.h"

/* Saves the sound heap state, so Sound_RestoreHeapState can later free
 * everything loaded after this point. */
void Sound_SaveHeapState(void) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndHeapSaveState(gSoundHeap);
}
