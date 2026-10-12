#include "types.h"
#include "game/heap.h"

/* Drops every allocation made from the end of the free region. The saved
 * states remember where the end was, so they are updated too. */
void FrameHeap_FreeTail(FrameHeap* heap) {
  FrameHeapState* state;
  FrameHeapData* data = &heap->data;

  for (state = data->states; state != NULL; state = state->prev) {
    state->tail = heap->head.end;
  }
  data->free.end = heap->head.end;
}
