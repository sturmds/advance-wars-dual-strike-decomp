#include "sdk/os.h"

u32 OSi_DoTryLockByWord(u16 lockID, OSLockWord* lockp, void (*ctrl)(void),
                        BOOL disableFiq) {
  OSIntrMode intr;
  u32 lockFlag;

  if (disableFiq) {
    intr = OS_DisableInterrupts_IrqAndFiq();
  } else {
    intr = OS_DisableInterrupts();
  }
  lockFlag = MI_SwapWord(lockID, &lockp->lockFlag);
  if (lockFlag == 0) {
    if (ctrl != NULL) {
      ctrl();
    }
    lockp->ownerID = lockID;
  }
  if (disableFiq) {
    OS_RestoreInterrupts_IrqAndFiq(intr);
  } else {
    OS_RestoreInterrupts(intr);
  }
  return lockFlag;
}
