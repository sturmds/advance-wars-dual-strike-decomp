#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

/*
 * ARM7 copy of OS_CheckHeap. Walks both block lists of a heap and checks
 * their links, alignment and sizes. Returns the free bytes (headers not
 * counted), or -1 if anything is wrong or the blocks do not add up to the
 * heap's size.
 */
s32 Heap_VerifyAndCountFree_Arm7(OSArenaId id, s32 heap)
{
    OSHeapInfo *info;
    HeapRecord *desc;
    HeapBlockHeader *cell;
    s32 total = 0;
    s32 freeBytes = 0;
    s32 result = -1;
    int irqState = Cpu_MaskIrq_Arm7();

    info = data_arm7_03807a54[id];
    if (info->heapArray == NULL) {
        goto done;
    }
    if (heap < 0 || heap >= info->numHeaps) {
        goto done;
    }
    desc = &info->heapArray[heap];
    if (desc->size < 0) {
        goto done;
    }

    cell = desc->allocated;
    if (cell != NULL && cell->prev != NULL) {
        goto done;
    }
    for (; cell != NULL; cell = cell->next) {
        if ((u32)info->arenaStart > (u32)cell || (u32)cell >= (u32)info->arenaEnd) {
            goto done;
        }
        if (((u32)cell & 31) != 0) {
            goto done;
        }
        if (cell->next != NULL && cell->next->prev != cell) {
            goto done;
        }
        if ((u32)cell->size < HEAP_SMALLEST_SPLIT || (cell->size & 31) != 0) {
            goto done;
        }
        total += cell->size;
        if (total <= 0 || total > desc->size) {
            goto done;
        }
    }

    cell = desc->free;
    if (cell != NULL && cell->prev != NULL) {
        goto done;
    }
    for (; cell != NULL; cell = cell->next) {
        if ((u32)info->arenaStart > (u32)cell || (u32)cell >= (u32)info->arenaEnd) {
            goto done;
        }
        if (((u32)cell & 31) != 0) {
            goto done;
        }
        if (cell->next != NULL && cell->next->prev != cell) {
            goto done;
        }
        if ((u32)cell->size < HEAP_SMALLEST_SPLIT || (cell->size & 31) != 0) {
            goto done;
        }
        /* free blocks are merged, so two of them never touch */
        if (cell->next != NULL && (u32)cell + cell->size >= (u32)cell->next) {
            goto done;
        }
        total += cell->size;
        freeBytes += cell->size - HEAP_BLOCK_HEADER_BYTES;
        if (total <= 0 || total > desc->size) {
            goto done;
        }
    }

    if (total == desc->size) {
        result = freeBytes;
    }
done:
    Cpu_RestoreIrqBit_Arm7(irqState);
    return result;
}
