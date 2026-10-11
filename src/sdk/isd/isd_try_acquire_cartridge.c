/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

#define CARTRIDGE_LOCK_WORD ((void *)0x027fffe8)

int OS_LockCartridge(int lockId);
u16 func_0210cf88(void *lock);
BOOL func_02121ab8(void);
BOOL func_02121b04(void);
void func_02121ac8(BOOL state);
void func_02121b14(BOOL state);

/*
 * Tries to take the cartridge lock (id 0x70) and, if the bus is ours, stores in *saved
 * what the two helper calls returned (bit 0, bit 1) and whether we took the lock (0x20).
 * On failure it undoes the two helpers and stores 0. Returns whether the bus is ours.
 */
BOOL Isd_TryAcquireCartridge(u32 *saved)
{
    BOOL savedB = func_02121b04();
    BOOL savedA = func_02121ab8();
    BOOL locked = FALSE;
    BOOL lockedByUs = FALSE;

    if (OS_LockCartridge(0x70) == 0) {
        lockedByUs = TRUE;
        locked = lockedByUs;
    } else if (func_0210cf88(CARTRIDGE_LOCK_WORD) & 0x40) {
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
        func_02121ac8(savedA);
        func_02121b14(savedB);
        *saved = 0;
    }
    return locked;
}
