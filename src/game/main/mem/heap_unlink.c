#include "types.h"
#include "game/heap.h"

LinkList *Heap_FindParentList(HeapHead *heap);
void func_02104150(LinkList *list, void *object);

/* Takes a heap out of the list that holds it, before the heap goes away. */
void Heap_Unlink(HeapHead *heap)
{
    func_02104150(Heap_FindParentList(heap), heap);
}
