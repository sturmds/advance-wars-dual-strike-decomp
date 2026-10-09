#ifndef GAME_FRAME_HEAP_H
#define GAME_FRAME_HEAP_H

#include "types.h"

/* Frame heap (NitroSystem DoubleEndedHeap): allocations grow from both ends
 * and are dropped back to a saved state. */
typedef struct FrameHeapState {
    u32 tag;                           /* number given when the state was saved */
    void *head;                        /* allocation pointers at that time */
    void *tail;
    struct FrameHeapState *prev;       /* the state saved before this one */
} FrameHeapState;

/* The part of the heap that moves. */
typedef struct FrameHeapCursor {
    void *head;                        /* next free byte from the bottom */
    void *tail;                        /* end of the free space; top allocations lie above */
    FrameHeapState *lastState;
} FrameHeapCursor;

typedef struct FrameHeap {
    u8 pad_00[0x24];                   /* generic heap header */
    FrameHeapCursor cursor;
} FrameHeap;

#endif
