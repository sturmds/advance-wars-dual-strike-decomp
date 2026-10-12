#include "sdk/os.h"

int func_arm7_037fdb80(void);                           /* ARM7 OS_DisableInterrupts */
void func_arm7_037fdb94(int state);                     /* ARM7 OS_RestoreInterrupts */
OSIntrMode func_arm7_037fdbac(void);                    /* ARM7 OS_DisableInterrupts_IrqAndFiq */
OSIntrMode func_arm7_037fdbc0(OSIntrMode state);        /* ARM7 OS_RestoreInterrupts_IrqAndFiq */

/* ARM7 copy of OSi_DoUnlockByWord; see the ARM9 declaration in sdk/os.h. */
int func_arm7_037fb9e8(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq)
{
    OSIntrMode intr;

    if (lockID != lockp->ownerID) {
        return LOCK_NOT_OWNER;
    }
    if (disableFiq) {
        intr = func_arm7_037fdbac();
    } else {
        intr = func_arm7_037fdb80();
    }
    lockp->ownerID = 0;
    if (ctrl != NULL) {
        ctrl();
    }
    lockp->lockFlag = 0;
    if (disableFiq) {
        func_arm7_037fdbc0(intr);
    } else {
        func_arm7_037fdb94(intr);
    }
    return 0;
}
