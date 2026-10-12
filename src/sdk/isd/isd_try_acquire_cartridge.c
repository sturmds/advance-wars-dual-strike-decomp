/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

#define CARTRIDGE_LOCK_WORD ((void *)0x027fffe8)

int OS_TryLockCartridge(u16 lockId);
u16 OS_ReadOwnerOfLockWord(void *lock);

/*
 * Tries to take the cartridge lock (id 0x70) and, if the bus is ours, stores in *saved
 * what the two helper calls returned (bit 0, bit 1) and whether we took the lock (0x20).
 * On failure it undoes the two helpers and stores 0. Returns whether the bus is ours.
 */
BOOL Isd_TryAcquireCartridge(u32 *saved)
{
    BOOL savedB = Isd_DisableIrq();
    BOOL savedA = Isd_DisableFiq();
    BOOL locked = FALSE;
    BOOL lockedByUs = FALSE;

    if (OS_TryLockCartridge(0x70) == 0) {
        lockedByUs = TRUE;
        locked = lockedByUs;
    } else if (OS_ReadOwnerOfLockWord(CARTRIDGE_LOCK_WORD) & 0x40) {
        locked = TRUE;
    }
    if (locked) {
        u32 result = 0;

        if (savedB) {
            result |= 1;
        }
        if (savedA) {
            result |= 2;
        }
        if (lockedByUs) {
            result |= 0x20;
        }
        *saved = result;
    } else {
        Isd_RestoreFiq(savedA);
        Isd_RestoreIrq(savedB);
        *saved = 0;
    }
    return locked;
}
