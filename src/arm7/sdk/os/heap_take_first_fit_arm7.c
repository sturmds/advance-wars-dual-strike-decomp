#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
HeapBlockHeader *Heap_UnlinkBlock(HeapBlockHeader *list, HeapBlockHeader *cell);
HeapBlockHeader *Heap_LinkAtFront(HeapBlockHeader *list, HeapBlockHeader *cell);

/*
 * ARM7 copy of OS_AllocFromHeap. Takes the first free block big enough for
 * size bytes plus a header, splits off the rest when it can hold another
 * block, and returns the memory after the header. NULL if the arena has no
 * heaps or nothing fits. A negative heap means the current one.
 */
void *Heap_TakeFirstFit_Arm7(OSArenaId id, s32 heap, s32 size)
{
    OSHeapInfo *info;
    HeapRecord *desc;
    HeapBlockHeader *cell;
    HeapBlockHeader *rest;
    s32 leftover;
    int irqState;

    irqState = func_arm7_037fdb80();
    info = data_arm7_03807a54[id];
    if (info == NULL) {
        func_arm7_037fdb94(irqState);
        return NULL;
    }
    if (heap < 0) {
        heap = info->currentHeap;
    }
    desc = &info->heapArray[heap];
    /* needed to match: the u32 cast keeps the header add and the rounding add apart */
    size = ((u32)(size + HEAP_BLOCK_HEADER_BYTES) + 31) & ~31;

    for (cell = desc->free; cell != NULL; cell = cell->next) {
        if (size <= cell->size) {
            break;
        }
    }
    if (cell == NULL) {
        func_arm7_037fdb94(irqState);
        return NULL;
    }

    leftover = cell->size - size;
    if ((u32)leftover < HEAP_SMALLEST_SPLIT) {
        desc->free = Heap_UnlinkBlock(desc->free, cell);
    } else {
        cell->size = size;
        rest = (HeapBlockHeader *)((u8 *)cell + size);
        rest->size = leftover;
        rest->prev = cell->prev;
        rest->next = cell->next;
        if (rest->next != NULL) {
            rest->next->prev = rest;
        }
        if (rest->prev != NULL) {
            rest->prev->next = rest;
        } else {
            desc->free = rest;
        }
    }
    desc->allocated = Heap_LinkAtFront(desc->allocated, cell);
    func_arm7_037fdb94(irqState);
    return (u8 *)cell + HEAP_BLOCK_HEADER_BYTES;
}
