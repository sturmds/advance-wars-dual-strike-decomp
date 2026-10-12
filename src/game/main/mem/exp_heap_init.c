#include "types.h"
#include "game/heap.h"

#define EXP_HEAP_SIGNATURE 0x45585048 /* "EXPH" */

/* Sets up an expanded heap in [start, end): the header, then one free block
 * that covers the rest. */
ExpHeap* ExpHeap_Init(void* start, void* end, u16 options) {
  ExpHeap* heap = start;
  ExpHeapData* data = &heap->data;
  HeapRegion region;
  HeapBlock* freeBlock;

  Heap_InitHead(&heap->head, EXP_HEAP_SIGNATURE, data + 1, end, options);
  data->groupId = 0;
  data->allocMode = 0;
  data->allocMode &= ~1;

  region.start = heap->head.start;
  region.end = heap->head.end;
  freeBlock = HeapBlock_InitInRegion(&region, HEAP_BLOCK_FREE);
  data->freeBlocks.head = freeBlock;
  data->freeBlocks.tail = freeBlock;
  data->usedBlocks.head = NULL;
  data->usedBlocks.tail = NULL;
  return heap;
}
