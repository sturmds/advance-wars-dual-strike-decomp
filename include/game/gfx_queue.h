#ifndef GAME_GFX_QUEUE_H
#define GAME_GFX_QUEUE_H

#include "types.h"

/* A graphics copy queued for the next frame (read by GfxQueue_Flush, called
 * from MainLoop_DefaultVBlank). Not the same queue as TransferEntry.
 * The volatile qualifiers are needed to match the code that fills entries. */
typedef struct GfxQueueEntry {
  const void* src;
  void* volatile dst;
  volatile u32 size; /* bytes */
  volatile u8
      kind; /* 0: free; 1, 3: texture image; 2, 4: texture palette (GfxQueue_Flush) */
  u8 pad_0d[3];
} GfxQueueEntry;

#define GFX_QUEUE_LENGTH 32
/* Kinds 3 and 4 own a heap copy in src; it is freed after the copy. */
#define GFX_QUEUE_OWNS_SRC(kind) ((kind) == 3 || (kind) == 4)

extern volatile u32 data_027c0518; /* bytes queued */
extern volatile int data_027c051c; /* entries queued */
extern GfxQueueEntry data_027c0520[];

#endif
