#include "types.h"
#include "game/heap.h"

/* Unlinks a block from a list. Returns the block that was before it. */
HeapBlock* HeapBlockList_Remove(HeapBlockList* list, HeapBlock* block) {
  HeapBlock* prev = block->prev;
  HeapBlock* next = block->next;

  if (prev != NULL) {
    prev->next = next;
  } else {
    list->head = next;
  }
  if (next != NULL) {
    next->prev = prev;
  } else {
    list->tail = prev;
  }
  return prev;
}
