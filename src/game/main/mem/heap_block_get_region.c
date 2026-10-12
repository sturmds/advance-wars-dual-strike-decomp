#include "types.h"
#include "game/heap.h"

/* Gives the whole range a block takes: its alignment padding, the header
 * and the memory. */
void HeapBlock_GetRegion(HeapRegion* region, HeapBlock* block) {
  region->start = (void*)((u32)block - (u16)HEAP_BLOCK_PADDING(block));
  region->end = (void*)(block->size + (u32)(block + 1));
}
