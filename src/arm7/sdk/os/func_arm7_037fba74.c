#include "sdk/os.h"

void func_arm7_037fb8e4(void);   /* spins for 0x1000 cycles */
u32 func_arm7_037fb964(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);   /* ARM7 OSi_DoLockByWord */

/*
 * Takes a lock word, waiting until it is free: retries the ARM7
 * OSi_DoLockByWord while it returns another owner's lock flag. Returns the
 * last lock flag (0 once the lock is ours).
 */
s32 func_arm7_037fba74(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq)
{
    s32 lockFlag;

    while ((lockFlag = func_arm7_037fb964(lockID, lockp, ctrl, disableFiq)) > 0) {
        func_arm7_037fb8e4();
    }
    return lockFlag;
}
