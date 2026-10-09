#include "types.h"
#include "game/heap.h"

HeapHead *func_02104424(LinkList *list, void *address);

extern LinkList gHeapRootList;

/* Returns the list a heap belongs in: the child list of the innermost heap
 * whose range holds it, or the root list. */
LinkList *Heap_FindParentList(HeapHead *heap)
{
    LinkList *list = &gHeapRootList;
    HeapHead *parent = func_02104424(list, heap);

    if (parent != NULL) {
        list = &parent->children;
    }
    return list;
}
