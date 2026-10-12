#include "types.h"
#include "game/heap.h"

/* Allocates from the top of the heap: searches the free list from the end
 * and places the block at the top of the chosen free block. In first-fit
 * mode it takes the first block that fits; otherwise the smallest one. */
void* Heap_AllocFromTop(ExpHeap* heap, u32 size, int align) {
  ExpHeapData* data = &heap->data;
  /* needed to match: the mode bit is read as a u16 */
  BOOL firstFit = (u16)(data->allocMode & 1) == 0;
  HeapBlock* found = NULL;
  HeapBlock* block;
  u32 foundSize = 0xFFFFFFFF;
  void* foundMemory = NULL;

  for (block = data->freeBlocks.tail; block != NULL; block = block->prev) {
    void* memory = block + 1;
    void* end = (void*)(block->size + (u32)memory);
    void* start = (void*)(((u32)end - size) & ~(align - 1));

    /* the block fits when start does not fall below its memory */
    if ((s32)((u32)start - (u32)memory) >= 0 && foundSize > block->size) {
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
  return Heap_TakeFromFreeBlock(data, found, foundMemory, size, 1);
}
