#include "types.h"
#include "game/heap.h"

/* Allocates from the bottom of the heap: searches the free list from the
 * start and places the block at the bottom of the chosen free block. In
 * first-fit mode it takes the first block that fits; otherwise the
 * smallest one. */
void* Heap_AllocFromBottom(ExpHeap* heap, u32 size, int align) {
  ExpHeapData* data = &heap->data;
  /* needed to match: the mode bit is read as a u16 */
  BOOL firstFit = (u16)(data->allocMode & 1) == 0;
  HeapBlock* found = NULL;
  HeapBlock* block;
  u32 foundSize = 0xFFFFFFFF;
  void* foundMemory = NULL;

  for (block = data->freeBlocks.head; block != NULL; block = block->next) {
    void* memory = block + 1;
    void* start = (void*)(((align - 1) + (u32)memory) & ~(align - 1));

    /* the block fits when size bytes remain after the alignment gap */
    if (block->size >= size + ((u32)start - (u32)memory) &&
        foundSize > block->size) {
      found = block;
      foundSize = block->size;
      foundMemory = start;
      if (firstFit || foundSize == size) {
        break;
      }
    }
  }

  if (found == NULL) {
    return NULL;
  }
  return Heap_TakeFromFreeBlock(data, found, foundMemory, size, 0);
}
