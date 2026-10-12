#include "types.h"
#include "sdk/os_arm7.h"

/* Unlinks cell from a heap's free or used list. Returns the new head of the list. */
HeapBlockHeader *Heap_UnlinkBlock(HeapBlockHeader *list, HeapBlockHeader *cell)
{
    if (cell->next != NULL) {
        cell->next->prev = cell->prev;
    }
    if (cell->prev == NULL) {
        list = cell->next;
    } else {
        cell->prev->next = cell->next;
    }
    return list;
}
