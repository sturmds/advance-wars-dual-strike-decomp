#include "types.h"
#include "game/heap.h"

void MIi_CpuClear32(u32 value, void* dest,
                    u32 size); /* fills [dest, dest + size) with value */

#define HEAP_BLOCK_USED 0x5544 /* "UD" */

static inline void HeapBlock_SetFromTop(HeapBlock* block, u16 fromTop) {
  block->attribute &= ~0x8000;
  block->attribute |= (fromTop & 1) << 15;
}

static inline void HeapBlock_SetPadding(HeapBlock* block, u16 padding) {
  block->attribute &= ~0x7F00;
  block->attribute |= (padding & 0x7F) << 8;
}

static inline void HeapBlock_SetGroupId(HeapBlock* block, u8 groupId) {
  block->attribute &= ~0xFF;
  block->attribute |= groupId;
}

/* Cuts a used block out of a free block. The used block's memory starts at
 * memory. What is left of the free block in front of it and behind it goes
 * back on the free list, unless it is too small for a header; then it
 * stays inside the used block as padding in front or slack behind. */
void* Heap_TakeFromFreeBlock(ExpHeapData* data, HeapBlock* freeBlock,
                             void* memory, u32 size, u16 fromTop) {
  HeapRegion front;
  HeapRegion back;
  HeapRegion used;
  HeapBlock* prev;
  HeapBlock* block;
  volatile u32 fill;
  u32 usedSize;
  u8* blockStart;

  HeapBlock_GetRegion(&front, freeBlock);
  blockStart = (u8*)memory - sizeof(HeapBlock);
  back.end = front.end;
  front.end = blockStart;
  back.start = (void*)(size + (u32)memory);

  prev = HeapBlockList_Remove(&data->freeBlocks, freeBlock);

  if ((u32)front.end - (u32)front.start < sizeof(HeapBlock)) {
    front.end = front.start;
  } else {
    prev = HeapBlockList_Insert(&data->freeBlocks,
                                HeapBlock_InitInRegion(&front, HEAP_BLOCK_FREE),
                                prev);
  }

  if ((u32)back.end - (u32)back.start < sizeof(HeapBlock)) {
    back.start = back.end;
  } else {
    HeapBlockList_Insert(&data->freeBlocks,
                         HeapBlock_InitInRegion(&back, HEAP_BLOCK_FREE), prev);
  }

  usedSize = (u32)back.start - (u32)front.end;
  /* option bit 0: fill the new block with 0 */
  if ((u16)(((ExpHeap*)((u32)data - sizeof(HeapHead)))->head.options & 0xFF) &
      1) {
    fill = 0;
    MIi_CpuClear32(fill, front.end, usedSize);
  }

  used.start = blockStart;
  used.end = back.start;
  block = HeapBlock_InitInRegion(&used, HEAP_BLOCK_USED);
  HeapBlock_SetFromTop(block, fromTop);
  HeapBlock_SetPadding(block, (u32)block - (u32)front.end);
  HeapBlock_SetGroupId(block, data->groupId);
  HeapBlockList_Insert(&data->usedBlocks, block, data->usedBlocks.tail);
  return memory;
}
