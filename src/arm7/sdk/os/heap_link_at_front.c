#include "types.h"
#include "sdk/os_arm7.h"

/* Puts cell at the front of a heap list. Returns cell, the new head. */
HeapBlockHeader* Heap_LinkAtFront(HeapBlockHeader* list,
                                  HeapBlockHeader* cell) {
  cell->next = list;
  cell->prev = NULL;
  if (list != NULL) {
    list->prev = cell;
  }
  return cell;
}
