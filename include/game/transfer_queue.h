#ifndef GAME_TRANSFER_QUEUE_H
#define GAME_TRANSFER_QUEUE_H

#include "types.h"

#define TRANSFER_QUEUE_SIZE 48

/* A copy queued for the next frame update (func_020038fc runs the queue).
 * The volatile qualifiers are needed to match the code that fills entries. */
typedef struct TransferEntry {
    const void *src;
    void *dst;
    volatile u16 size;                 /* bytes */
    volatile u8 kind;                  /* 0 = copy, 4 = other (see func_02003b44) */
    u8 pad_0b;
} TransferEntry;

extern volatile int data_0216e04c;     /* entries queued */
extern volatile int data_0216e050;     /* bytes queued */
extern TransferEntry data_0216e05c[TRANSFER_QUEUE_SIZE];

#endif
