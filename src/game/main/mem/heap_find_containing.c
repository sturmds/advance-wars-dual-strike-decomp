#include "types.h"
#include "game/heap.h"

void *func_02104138(LinkList *list, void *object);

/* Returns the innermost heap in the list (or below it) whose range holds
 * address, or NULL. */
HeapHead *Heap_FindContaining(LinkList *list, void *address)
{
    HeapHead *heap = NULL;

    while ((heap = func_02104138(list, heap)) != NULL) {
        if (heap->start <= address && address < heap->end) {
            HeapHead *child = Heap_FindContaining(&heap->children, address);

            if (child == NULL) {
                child = heap;
            }
            return child;
        }
    }
    return NULL;
}
