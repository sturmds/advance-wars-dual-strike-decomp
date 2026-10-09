#include "types.h"
#include "game/heap.h"

HeapBlock *func_02104a00(HeapRegion *region, u16 signature);
HeapBlock *func_02104a60(HeapBlockList *list, HeapBlock *block);
HeapBlock *func_02104a30(HeapBlockList *list, HeapBlock *block, HeapBlock *prev);

/* Puts a region back on the free list, merged with the free blocks right
 * before and after it. The free list is sorted by address. Returns FALSE
 * when the result is too small to hold a block header. */
BOOL Heap_ReturnRegion(ExpHeapData *data, HeapRegion *region)
{
    HeapBlock *prevFree = NULL;
    HeapRegion merged = *region;
    HeapBlock *block;

    for (block = data->freeBlocks.head; block != NULL; block = block->next) {
        if ((void *)block < region->start) {
            prevFree = block;
            continue;
        }
        if ((void *)block == region->end) {
            merged.end = HEAP_BLOCK_END(block);
            func_02104a60(&data->freeBlocks, block);
        }
        break;
    }

    if (prevFree != NULL && HEAP_BLOCK_END(prevFree) == region->start) {
        merged.start = prevFree;
        prevFree = func_02104a60(&data->freeBlocks, prevFree);
    }

    if ((u32)merged.end - (u32)merged.start < sizeof(HeapBlock)) {
        return FALSE;
    }
    func_02104a30(&data->freeBlocks, func_02104a00(&merged, HEAP_BLOCK_FREE), prevFree);
    return TRUE;
}
