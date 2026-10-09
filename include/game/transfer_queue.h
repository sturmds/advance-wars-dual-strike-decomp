#ifndef GAME_TRANSFER_QUEUE_H
#define GAME_TRANSFER_QUEUE_H

#include "types.h"

#define TRANSFER_QUEUE_SIZE 48

/* A copy queued for the next frame update (func_020038fc runs the queue). */
typedef struct TransferEntry {
    const void *src;
    void *dst;
    u16 size;                          /* bytes */
    u8 kind;                           /* 0 = copy, 4 = other (see func_02003b44) */
    u8 pad_0b;
} TransferEntry;

extern int data_0216e04c;                        /* entries queued */
extern TransferEntry data_0216e05c[TRANSFER_QUEUE_SIZE];

#endif
