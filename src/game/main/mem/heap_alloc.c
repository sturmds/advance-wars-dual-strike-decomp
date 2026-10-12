#include "types.h"
#include "game/heap.h"

void* Heap_AllocFromTop(ExpHeap* heap, u32 size, int align);

/* Allocates size bytes, rounded up to 4, aligned to |align|. A positive
 * align takes memory from the bottom of the heap, a negative one from the
 * top. */
void* Heap_Alloc(ExpHeap* heap, u32 size, int align) {
  if (size == 0) {
    size = 1;
  }
  size = (size + 3) & ~3;

  if (align >= 0) {
    return Heap_AllocFromBottom(heap, size, align);
  }
  return Heap_AllocFromTop(heap, size, -align);
}
