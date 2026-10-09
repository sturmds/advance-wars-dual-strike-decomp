#include "types.h"
#include "game/heap.h"

void *func_02104d0c(HeapRegion *free, u32 size, int align);

/* Saves the frame heap's current state under tag: pushes a FrameHeapState
 * recording both ends of the free region. Returns FALSE when there is no
 * room for the state. */
BOOL FrameHeap_SaveState(FrameHeap *heap, u32 tag)
{
    void *head = heap->data.free.start;
    FrameHeapData *data = &heap->data;
    FrameHeapState *state = func_02104d0c(&data->free, sizeof(FrameHeapState), 4);

    if (state == NULL) {
        return FALSE;
    }
    state->tag = tag;
    state->head = head;
    state->tail = data->free.end;
    state->prev = data->states;
    data->states = state;
    return TRUE;
}
