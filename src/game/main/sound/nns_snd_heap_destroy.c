#include "types.h"
#include "game/sound.h"
#include "game/sound_heap.h"

void func_02104bf0(void* frameHeap);
void SoundHeap_Reset(SoundHeap* heap);

/* NitroSystem NNS_SndHeapDestroy: unloads every item, then unlinks
 * the memory heap it lives in from the heap tree. */
void NNS_SndHeapDestroy(void* heapArg) {
  SoundHeap* heap = heapArg;

  SoundHeap_Reset(heap);
  func_02104bf0(heap->frameHeap);
}
