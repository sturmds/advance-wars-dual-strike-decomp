#include "types.h"
#include "game/heap.h"

/* Drops the heap back to the saved state with the given tag. A tag of 0
 * means the newest state. Returns FALSE when the state does not exist. */
BOOL FrameHeap_FreeByState(FrameHeap* heap, u32 tag) {
  FrameHeapState* state;
  FrameHeapData* data = &heap->data;

  state = data->states;

  if (tag != 0 && state != NULL) {
    do {
      if (state->tag == tag) {
        break;
      }
      state = state->prev;
    } while (state != NULL);
  }
  if (state == NULL) {
    return FALSE;
  }
  data->free.start = state->head;
  data->free.end = state->tail;
  data->states = state->prev;
  return TRUE;
}
