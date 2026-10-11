#ifndef SDK_MI_H
#define SDK_MI_H

#include "types.h"

/* One DMA channel's registers: source, destination, length and control. */
typedef struct DmaChannel {
    volatile u32 src;
    volatile u32 dest;
    volatile u32 control;
} DmaChannel;

/* The four channels start at 0x040000b0, 12 bytes apart. */
#define REG_DMA ((DmaChannel *)0x040000b0)

/* Control value for a one-unit transfer, 16-bit, fixed source and destination, starting at once. */
#define DMA_DUMMY_TRANSFER 0x81400001

#endif
