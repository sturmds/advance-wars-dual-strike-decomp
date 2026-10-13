#include "types.h"
#include "game/heap.h"

LinkList* Heap_FindParentList(HeapHead* heap);

static BOOL gHeapRootListReady;
/* also declared in src/game/main/mem/heap_find_parent_list.c */
LinkList gHeapRootList;

/* Fills in a new heap's header and adds the heap to the list of the heap
 * whose range holds it (or to the root list). */
void Heap_InitHead(HeapHead* heap, u32 signature, void* start, void* end,
                   u16 options) {
  heap->signature = signature;
  heap->start = start;
  heap->end = end;
  /* needed to match: cleared, then the option bits masked in, in separate steps */
  heap->options = 0;
  heap->options &= ~0xFF;
  heap->options |= options & 0xFF;

  LinkList_Init(&heap->children, HEAP_LINK_OFFSET);

  if (!gHeapRootListReady) {
    LinkList_Init(&gHeapRootList, HEAP_LINK_OFFSET);
    gHeapRootListReady = TRUE;
  }

  LinkList_Append(Heap_FindParentList(heap), heap);
}
