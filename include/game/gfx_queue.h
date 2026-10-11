#ifndef GAME_GFX_QUEUE_H
#define GAME_GFX_QUEUE_H

#include "types.h"

/* A graphics copy queued for the next frame (read by GfxQueue_Flush, called
 * from MainLoop_DefaultVBlank). Not the same queue as TransferEntry.
 * The volatile qualifiers are needed to match the code that fills entries. */
typedef struct GfxQueueEntry {
    const void *src;
    void *volatile dst;
    volatile u32 size;                 /* bytes */
    volatile u8 kind;                  /* 2, 3 and 4 are queued so far */
    u8 pad_0d[3];
} GfxQueueEntry;

extern volatile u32 data_027c0518;     /* bytes queued */
extern volatile int data_027c051c;     /* entries queued */
extern GfxQueueEntry data_027c0520[];

#endif
