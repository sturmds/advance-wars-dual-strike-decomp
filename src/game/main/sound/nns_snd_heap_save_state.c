#include "types.h"
#include "game/sound.h"
#include "game/sound_heap.h"

int FrameHeap_FreeByState(void* frameHeap, int level);
int FrameHeap_SaveState(void* frameHeap, int level);
int SoundHeap_AddLevel(SoundHeap* heap);

/* NitroSystem NNS_SndHeapSaveState: starts a new save point and returns its
 * level, or -1 when there is no room for it. */
int NNS_SndHeapSaveState(void* heapArg) {
  SoundHeap* heap = heapArg;

  if (FrameHeap_SaveState(heap->frameHeap, heap->levels.count) == 0) {
    return -1;
  }
  if (SoundHeap_AddLevel(heap) != 0) {
    return heap->levels.count - 1;
  }
  FrameHeap_FreeByState(heap->frameHeap, 0);
  return -1;
}
