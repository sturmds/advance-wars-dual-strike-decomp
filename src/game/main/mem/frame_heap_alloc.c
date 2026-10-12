#include "types.h"
#include "game/heap.h"

/* Allocates from the head end, or from the tail end when align is negative.
 * A size of 0 is treated as 1 and sizes round up to 4. */
void* FrameHeap_Alloc(FrameHeap* heap, u32 size, int align) {
  FrameHeapData* data = &heap->data;

  if (size == 0) {
    size = 1;
  }
  size = (size + 3) & ~3;
  if (align >= 0) {
    return FrameHeap_AllocFromHead(data, size, align);
  }
  return FrameHeap_AllocFromTail(data, size, -align);
}
