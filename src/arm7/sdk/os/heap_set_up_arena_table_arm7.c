#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/*
 * ARM7 copy of OS_InitAlloc. Sets up heap bookkeeping for an arena at start:
 * the OSHeapInfo, then maxHeaps unused heap slots. Returns the first byte
 * after them, 32-byte aligned; heaps are made from there to end.
 */
void *Heap_SetUpArenaTable_Arm7(ArenaRegionArm7 id, void *start, void *end, s32 maxHeaps)
{
    int irqState = func_arm7_037fdb80();
    OSHeapInfo *info = start;
    HeapRecord *desc;
    s32 heap;
    u32 arraySize;

    data_arm7_03807a54[id] = info;
    arraySize = maxHeaps * sizeof(HeapRecord);
    info->heapArray = (HeapRecord *)(info + 1);
    info->numHeaps = maxHeaps;
    for (heap = 0; heap < info->numHeaps; heap++) {
        desc = &info->heapArray[heap];
        desc->size = -1;
        desc->free = desc->allocated = NULL;
    }
    info->currentHeap = -1;
    info->arenaStart = (void *)(((u32)info->heapArray + arraySize + 31) & ~31);
    info->arenaEnd = (void *)((u32)end & ~31);
    func_arm7_037fdb94(irqState);
    return info->arenaStart;
}
