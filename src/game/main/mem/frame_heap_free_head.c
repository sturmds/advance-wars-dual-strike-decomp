#include "types.h"
#include "game/heap.h"

/* Drops every allocation made from the start of the free region and all saved states. */
void FrameHeap_FreeHead(FrameHeap* heap) {
  FrameHeapData* data = &heap->data;

  data->free.start = heap->head.start;
  data->states = NULL;
}
