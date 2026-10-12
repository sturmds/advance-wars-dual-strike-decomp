#include "types.h"
#include "game/heap.h"

/* Drops all allocations from the ends named by flags (FRAME_HEAP_FREE_HEAD,
 * FRAME_HEAP_FREE_TAIL). */
void FrameHeap_Free(FrameHeap* heap, int flags) {
  if (flags & FRAME_HEAP_FREE_HEAD) {
    FrameHeap_FreeHead(heap);
  }
  if (flags & FRAME_HEAP_FREE_TAIL) {
    FrameHeap_FreeTail(heap);
  }
}
