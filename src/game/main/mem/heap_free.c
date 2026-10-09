#include "types.h"
#include "game/heap.h"

void HeapBlock_GetRegion(HeapRegion *region, HeapBlock *block);
HeapBlock *HeapBlockList_Remove(HeapBlockList *list, HeapBlock *block);
BOOL Heap_ReturnRegion(ExpHeapData *data, HeapRegion *region);

/* Returns a block from Heap_Alloc to the heap's free memory. */
void Heap_Free(ExpHeap *heap, void *memory)
{
    ExpHeapData *data = &heap->data;
    HeapBlock *block = (HeapBlock *)((u8 *)memory - sizeof(HeapBlock));
    HeapRegion region;

    HeapBlock_GetRegion(&region, block);
    HeapBlockList_Remove(&data->usedBlocks, block);
    Heap_ReturnRegion(data, &region);
}
