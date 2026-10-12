#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
HeapBlockHeader *Heap_UnlinkBlock(HeapBlockHeader *list, HeapBlockHeader *cell);
HeapBlockHeader *Heap_MergeIntoFreeList(HeapBlockHeader *list, HeapBlockHeader *cell);

/* ARM7 copy of OS_FreeToHeap. Moves the block at ptr from the heap's used list to its free list. A negative heap means the current one. */
void Heap_ReturnBlock_Arm7(ArenaRegionArm7 id, s32 heap, void *ptr)
{
    OSHeapInfo *info;
    HeapRecord *desc;
    HeapBlockHeader *cell;
    int irqState;

    irqState = func_arm7_037fdb80();
    info = data_arm7_03807a54[id];
    if (heap < 0) {
        heap = info->currentHeap;
    }
    cell = (HeapBlockHeader *)((u8 *)ptr - HEAP_BLOCK_HEADER_BYTES);
    desc = &info->heapArray[heap];
    desc->allocated = Heap_UnlinkBlock(desc->allocated, cell);
    desc->free = Heap_MergeIntoFreeList(desc->free, cell);
    func_arm7_037fdb94(irqState);
}
