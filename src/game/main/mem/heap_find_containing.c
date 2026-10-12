#include "types.h"
#include "game/heap.h"

/* Returns the innermost heap in the list (or below it) whose range holds
 * address, or NULL. */
HeapHead* Heap_FindContaining(LinkList* list, void* address) {
  HeapHead* heap = NULL;

  while ((heap = LinkList_GetNext(list, heap)) != NULL) {
    if (heap->start <= address && address < heap->end) {
      HeapHead* child = Heap_FindContaining(&heap->children, address);

      if (child == NULL) {
        child = heap;
      }
      return child;
    }
  }
  return NULL;
}
