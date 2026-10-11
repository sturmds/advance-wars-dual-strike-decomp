#ifndef SDK_CTRDG_H
#define SDK_CTRDG_H

#include "types.h"

/* PXI FIFO tag of the cartridge library: the ARM9 and ARM7 exchange commands under it. */
#define FIFO_TAG_GBA_SLOT 13

/* Record of one lock on the cartridge bus, filled in by Cartridge_SpinUntilSlotOwned. */
typedef struct GbaSlotLockRecord {
    u32 heldBefore;                /* nonzero when an ARM9 lock ID already held the bus */
    int savedIrqState;             /* OS_DisableInterrupts result, restored on unlock */
} GbaSlotLockRecord;

/* Busy-waits for roughly the given number of loop passes (Thumb code in the SDK). */
void WaitByLoop(u32 count);

#endif
