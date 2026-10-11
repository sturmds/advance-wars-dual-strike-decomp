#include "sdk/os.h"

/*
 * Probably OS_ReleaseLockID: lock IDs 0x40 to 0x7f are tracked in two words
 * in shared main RAM, a set bit means the ID is free, and ID 0x40 is the top
 * bit of the first word. func_0210cef0 clears a bit to hand out an ID.
 *
 * Not matching: the original compares with pl/mi instead of ge/lt and
 * shifts before it loads the word.
 */
void func_0210cf48(int lockID)
{
    volatile u32 *freeMap = LOCK_ID_AVAILABLE_BITS;

    if (lockID >= 0x60) {
        freeMap++;
        lockID -= 0x60;
    } else {
        lockID -= 0x40;
    }
    *freeMap |= 0x80000000 >> lockID;
}
