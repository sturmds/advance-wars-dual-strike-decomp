#include "types.h"
#include "game/heap.h"

/* Links a block into a list right after prev. A NULL prev puts it first.
 * Returns the block. */
HeapBlock* HeapBlockList_Insert(HeapBlockList* list, HeapBlock* block,
                                HeapBlock* prev) {
  HeapBlock* next;

  block->prev = prev;
  if (prev != NULL) {
    next = prev->next;
    prev->next = block;
  } else {
    next = list->head;
    list->head = block;
  }
  block->next = next;
  if (next != NULL) {
    next->prev = block;
  } else {
    list->tail = block;
  }
  return block;
}
