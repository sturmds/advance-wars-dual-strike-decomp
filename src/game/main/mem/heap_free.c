#include "types.h"
#include "game/heap.h"

void func_02104a88(HeapRegion *region, HeapBlock *block);
HeapBlock *func_02104a60(HeapBlockList *list, HeapBlock *block);
BOOL Heap_ReturnRegion(ExpHeapData *data, HeapRegion *region);

/* Returns a block from Heap_Alloc to the heap's free memory. */
void Heap_Free(ExpHeap *heap, void *memory)
{
    ExpHeapData *data = &heap->data;
    HeapBlock *block = (HeapBlock *)((u8 *)memory - sizeof(HeapBlock));
    HeapRegion region;

    func_02104a88(&region, block);
    func_02104a60(&data->usedBlocks, block);
    Heap_ReturnRegion(data, &region);
}
