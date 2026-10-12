#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
HeapBlockHeader *func_arm7_037fce28(HeapBlockHeader *list, HeapBlockHeader *cell);
HeapBlockHeader *func_arm7_037fcd74(HeapBlockHeader *list, HeapBlockHeader *cell);

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
    desc->allocated = func_arm7_037fce28(desc->allocated, cell);
    desc->free = func_arm7_037fcd74(desc->free, cell);
    func_arm7_037fdb94(irqState);
}
