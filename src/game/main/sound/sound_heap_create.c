#include "types.h"
#include "game/sound_heap.h"

void func_02104bf0(void* frameHeap);
void* FrameHeap_Create(u32 start, u32 size, int flags);
BOOL SoundHeap_Init(SoundHeap* heap, void* frameHeap);

/* Creates a sound heap in the memory at mem. The SoundHeap sits at the
 * aligned start (16 bytes), the frame heap takes the rest. Returns NULL
 * when the memory is too small. */
SoundHeap* SoundHeap_Create(u32 mem, u32 size) {
  u32 end = mem + size;
  SoundHeap* heap = (SoundHeap*)((mem + 3) & ~3);
  u32 left;
  void* frameHeap;

  if ((u32)heap > end) {
    return NULL;
  }
  left = end - (u32)heap;
  if (left < 16) {
    return NULL;
  }
  frameHeap = FrameHeap_Create((u32)heap + 16, left - 16, 0);
  if (frameHeap == NULL) {
    return NULL;
  }
  if (SoundHeap_Init(heap, frameHeap)) {
    return heap;
  }
  func_02104bf0(frameHeap);
  return NULL;
}
