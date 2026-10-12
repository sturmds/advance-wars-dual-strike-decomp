#include "sdk/os_arm7.h"
#include "sdk/mi.h"

/* One try at a lock word; see the declaration in sdk/os_arm7.h. */
u32 LockWord_SwapOnce_Arm7(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq)
{
    OSIntrMode intr;
    u32 lockFlag;

    if (disableFiq) {
        intr = Cpu_MaskIrqFiq_Arm7();
    } else {
        intr = Cpu_MaskIrq_Arm7();
    }
    lockFlag = Atomic_ExchangeWord_Arm7(lockID, &lockp->lockFlag);
    if (lockFlag == 0) {
        if (ctrl != NULL) {
            ctrl();
        }
        lockp->ownerID = lockID;
    }
    if (disableFiq) {
        Cpu_RestoreIrqFiqBits_Arm7(intr);
    } else {
        Cpu_RestoreIrqBit_Arm7(intr);
    }
    return lockFlag;
}
