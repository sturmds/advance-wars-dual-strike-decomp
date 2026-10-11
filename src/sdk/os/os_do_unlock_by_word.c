#include "sdk/os.h"

int OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq)
{
    OSIntrMode intr;

    if (lockID != lockp->ownerID) {
        return LOCK_NOT_OWNER;
    }
    if (disableFiq) {
        intr = OS_DisableInterrupts_IrqAndFiq();
    } else {
        intr = OS_DisableInterrupts();
    }
    lockp->ownerID = 0;
    if (ctrl != NULL) {
        ctrl();
    }
    lockp->lockFlag = 0;
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(intr);
    } else {
        OS_RestoreInterrupts(intr);
    }
    return 0;
}
