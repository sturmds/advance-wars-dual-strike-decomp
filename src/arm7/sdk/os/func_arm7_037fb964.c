#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);                           /* ARM7 OS_DisableInterrupts */
void func_arm7_037fdb94(int state);                     /* ARM7 OS_RestoreInterrupts */
OSIntrMode func_arm7_037fdbac(void);                    /* ARM7 OS_DisableInterrupts_IrqAndFiq */
OSIntrMode func_arm7_037fdbc0(OSIntrMode state);        /* ARM7 OS_RestoreInterrupts_IrqAndFiq */
u32 func_arm7_037fe098(u32 setData, volatile u32 *destp);   /* ARM7 MI_SwapWord */

/* ARM7 copy of OSi_DoLockByWord; see the ARM9 declaration in sdk/os.h. */
u32 func_arm7_037fb964(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq)
{
    OSIntrMode intr;
    u32 lockFlag;

    if (disableFiq) {
        intr = func_arm7_037fdbac();
    } else {
        intr = func_arm7_037fdb80();
    }
    lockFlag = func_arm7_037fe098(lockID, &lockp->lockFlag);
    if (lockFlag == 0) {
        if (ctrl != NULL) {
            ctrl();
        }
        lockp->ownerID = lockID;
    }
    if (disableFiq) {
        func_arm7_037fdbc0(intr);
    } else {
        func_arm7_037fdb94(intr);
    }
    return lockFlag;
}
