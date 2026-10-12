#include "types.h"
#include "game/heap.h"

LinkList* Heap_FindParentList(HeapHead* heap);

/* Takes a heap out of the list that holds it, before the heap goes away. */
void Heap_Unlink(HeapHead* heap) {
  LinkList_Remove(Heap_FindParentList(heap), heap);
}
