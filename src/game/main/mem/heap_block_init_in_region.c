#include "types.h"
#include "game/heap.h"

/* Writes a block header at the start of a region. The block takes the rest
 * of the region. */
HeapBlock* HeapBlock_InitInRegion(HeapRegion* region, u16 signature) {
  HeapBlock* block = region->start;

  block->signature = signature;
  block->attribute = 0;
  block->size = (u32)region->end - (u32)(block + 1);
  block->prev = NULL;
  block->next = NULL;
  return block;
}
