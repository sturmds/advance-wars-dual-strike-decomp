#include "types.h"
#include "game/heap.h"

LinkList *func_021043f8(HeapHead *heap);
void func_02104150(LinkList *list, void *object);

/* Takes a heap out of the list that holds it, before the heap goes away. */
void Heap_Unlink(HeapHead *heap)
{
    func_02104150(func_021043f8(heap), heap);
}
