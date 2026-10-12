#include "types.h"
#include "game/sound.h"

/* Frees what was loaded into the sound heap since the last saved state.
 * Level 0 (nothing saved) is left alone. */
void Sound_RestoreHeapState(void) {
  int level;

  if (gSoundReady == 0) {
    return;
  }
  level = SoundHeap_NewestSaveLevel(gSoundHeap);
  if (level == 0) {
    return;
  }
  NNS_SndHeapLoadState(gSoundHeap, level);
}
