#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/*
 * ARM7 copy of OS_CreateHeap. Makes a heap of the memory from start to end
 * (shrunk to 32-byte bounds) in the first unused slot of the arena's heap
 * array, as one free block. Returns the heap's handle, or -1 if every slot
 * is in use.
 */
s32 Heap_CarveFromRange_Arm7(ArenaRegionArm7 id, void *start, void *end)
{
    int irqState = func_arm7_037fdb80();
    OSHeapInfo *info = data_arm7_03807a54[id];
    HeapRecord *desc;
    HeapBlockHeader *cell;
    s32 heap;

    start = (void *)(((u32)start + 31) & ~31);
    end = (void *)((u32)end & ~31);

    for (heap = 0; heap < info->numHeaps; heap++) {
        desc = &info->heapArray[heap];
        if (desc->size < 0) {
            desc->size = (u32)end - (u32)start;
            cell = start;
            cell->prev = NULL;
            cell->next = NULL;
            cell->size = desc->size;
            desc->free = cell;
            desc->allocated = NULL;
            func_arm7_037fdb94(irqState);
            return heap;
        }
    }
    func_arm7_037fdb94(irqState);
    return -1;
}
