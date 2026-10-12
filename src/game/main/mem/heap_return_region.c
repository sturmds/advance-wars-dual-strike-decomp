#include "types.h"
#include "game/heap.h"

/* Puts a region back on the free list, merged with the free blocks right
 * before and after it. The free list is sorted by address. Returns FALSE
 * when the result is too small to hold a block header. */
BOOL Heap_ReturnRegion(ExpHeapData* data, HeapRegion* region) {
  HeapBlock* prevFree = NULL;
  HeapRegion merged = *region;
  HeapBlock* block;

  for (block = data->freeBlocks.head; block != NULL; block = block->next) {
    if ((void*)block < region->start) {
      prevFree = block;
      continue;
    }
    if ((void*)block == region->end) {
      merged.end = HEAP_BLOCK_END(block);
      HeapBlockList_Remove(&data->freeBlocks, block);
    }
    break;
  }

  if (prevFree != NULL && HEAP_BLOCK_END(prevFree) == region->start) {
    merged.start = prevFree;
    prevFree = HeapBlockList_Remove(&data->freeBlocks, prevFree);
  }

  if ((u32)merged.end - (u32)merged.start < sizeof(HeapBlock)) {
    return FALSE;
  }
  HeapBlockList_Insert(&data->freeBlocks,
                       HeapBlock_InitInRegion(&merged, HEAP_BLOCK_FREE),
                       prevFree);
  return TRUE;
}
