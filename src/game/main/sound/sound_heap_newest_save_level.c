#include "types.h"
#include "game/sound.h"
#include "game/sound_heap.h"

/* NitroSystem SoundHeap_NewestSaveLevel: index of the newest save point.
 * Level 0 always exists. */
int SoundHeap_NewestSaveLevel(void* heap) {
  return ((SoundHeap*)heap)->levels.count - 1;
}
