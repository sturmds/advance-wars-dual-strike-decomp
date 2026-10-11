#ifndef SDK_CTRDG_H
#define SDK_CTRDG_H

#include "types.h"

/* PXI FIFO tag of the cartridge library: the ARM9 and ARM7 exchange commands under it. */
#define FIFO_TAG_GBA_SLOT 13

/* Busy-waits for roughly the given number of loop passes (Thumb code in the SDK). */
void WaitByLoop(u32 count);

#endif
