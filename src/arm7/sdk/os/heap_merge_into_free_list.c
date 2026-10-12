#include "types.h"
#include "sdk/os_arm7.h"

/*
 * Inserts cell into a heap's free list, which is sorted by address, and
 * merges it with the blocks just after and just before it when they touch.
 * Returns the new head of the list.
 */
HeapBlockHeader *Heap_MergeIntoFreeList(HeapBlockHeader *list, HeapBlockHeader *cell)
{
    HeapBlockHeader *prev;
    HeapBlockHeader *next;

    for (next = list, prev = NULL; next != NULL; next = next->next) {
        if (cell <= next) {
            break;
        }
        prev = next;
    }

    cell->next = next;
    cell->prev = prev;
    if (next != NULL) {
        next->prev = cell;
        if ((u8 *)cell + cell->size == (u8 *)next) {
            cell->size += next->size;
            next = next->next;
            cell->next = next;
            if (next != NULL) {
                next->prev = cell;
            }
        }
    }
    if (prev != NULL) {
        prev->next = cell;
        if ((u8 *)prev + prev->size == (u8 *)cell) {
            prev->size += cell->size;
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
        }
        return list;
    }
    return cell;
}
