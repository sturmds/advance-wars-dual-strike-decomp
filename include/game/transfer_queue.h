#ifndef GAME_TRANSFER_QUEUE_H
#define GAME_TRANSFER_QUEUE_H

#include "types.h"

#define TRANSFER_QUEUE_SIZE 48

/* A copy queued for the next frame update (TransferQueue_Run runs the queue).
 * The volatile qualifiers are needed to match the code that fills entries. */
typedef struct TransferEntry {
    const void *src;
    void *dst;
    volatile u16 size;                 /* bytes */
    volatile u8 kind;                  /* TRANSFER_* below */
    u8 pad_0b;
} TransferEntry;

/* TransferEntry.kind; TransferQueue_Run treats kind 1 like kind 0 */
#define TRANSFER_COPY    0
#define TRANSFER_COPY16  2             /* copy in 16-bit units */
#define TRANSFER_UNPACK  3             /* Data_Unpack */
#define TRANSFER_CLEAR16 4             /* clear `size` bytes at `src`, 16-bit units */
#define TRANSFER_CLEAR32 5             /* clear `size` bytes at `src`, 32-bit units */

extern volatile int data_0216e04c;     /* entries queued */
extern volatile int data_0216e050;     /* bytes queued */
extern TransferEntry data_0216e05c[TRANSFER_QUEUE_SIZE];

#endif
