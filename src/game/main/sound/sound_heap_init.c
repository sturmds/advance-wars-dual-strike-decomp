#include "types.h"
#include "game/sound_heap.h"

/* Sets up a sound heap on a frame heap and adds its first level. Returns
 * FALSE when the frame heap has no room for the level. */
BOOL SoundHeap_Init(SoundHeap* heap, void* frameHeap) {
  LinkList_Init(&heap->levels, 12);
  heap->frameHeap = frameHeap;
  return SoundHeap_AddLevel(heap) != 0;
}
