#ifndef SDK_CTRDG_H
#define SDK_CTRDG_H

#include "types.h"

/* PXI FIFO tag of the cartridge library: the ARM9 and ARM7 exchange commands under it. */
#define FIFO_TAG_GBA_SLOT 13

/* The OSLockWord in shared main RAM that arbitrates the cartridge bus between the CPUs. */
#define GBA_SLOT_OWNER_WORD ((void *)0x027fffe8)

/* Record of one lock on the cartridge bus, filled in by Cartridge_SpinUntilSlotOwned. */
typedef struct GbaSlotLockRecord {
    u32 heldBefore;                /* nonzero when an ARM9 lock ID already held the bus */
    int savedIrqState;             /* OS_DisableInterrupts result, restored on unlock */
} GbaSlotLockRecord;

/* EXMEMCNT fields that set the GBA slot ROM access time. */
#define GBA_SLOT_INITIAL_WAIT_BITS 0xc
#define GBA_SLOT_INITIAL_WAIT_POS 2
#define GBA_SLOT_FOLLOWUP_WAIT_BIT 0x10
#define GBA_SLOT_FOLLOWUP_WAIT_POS 4

/* GBA slot ROM access time settings, as two values saved from or loaded into EXMEMCNT. */
typedef struct GbaSlotWaitStates {
    u32 romFirstAccess;            /* EXMEMCNT bits 2 to 3 */
    u32 romSecondAccess;           /* EXMEMCNT bit 4 */
} GbaSlotWaitStates;

/* Busy-waits for roughly the given number of loop passes (Thumb code in the SDK). */
void WaitByLoop(u32 count);

#endif
