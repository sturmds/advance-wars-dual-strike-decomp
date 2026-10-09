#include "types.h"
#include "game/frame_heap.h"

/* Drops the heap back to the saved state with the given tag. A tag of 0
 * means the newest state. Returns FALSE when the state does not exist. */
BOOL FrameHeap_FreeByState(FrameHeap *heap, u32 tag)
{
    FrameHeapState *state;
    FrameHeapCursor *cursor = &heap->cursor;

    state = cursor->lastState;

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
    cursor->head = state->head;
    cursor->tail = state->tail;
    cursor->lastState = state->prev;
    return TRUE;
}
