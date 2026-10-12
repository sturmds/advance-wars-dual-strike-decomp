#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

/* Sound archive loaders: each loads one item into the sound heap and
 * returns TRUE on success. */

/* Loads a sequence archive (firstItem) and a bank (secondItem) into the
 * sound heap, like Sound_LoadResidentData. */
void Sound_LoadArchiveItems(int firstItem, int secondItem) {
  if (gSoundReady == 0) {
    return;
  }
  NNS_SndArcLoadSeqArc(firstItem, gSoundHeap);
  NNS_SndArcLoadBank(secondItem, gSoundHeap);
}
