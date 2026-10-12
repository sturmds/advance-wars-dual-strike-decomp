#include "types.h"
#include "game/heap.h"

/* Allocates from the game heap with 32-byte alignment. A negative size
 * flips the alignment to -32, which Heap_Alloc reads as "from the top". */
void* Mem_Alloc(s32 size) {
  return Heap_Alloc(gGameHeap, size, size >= 0 ? 32 : -32);
}
