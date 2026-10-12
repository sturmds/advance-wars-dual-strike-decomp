#include "sdk/os_arm7.h"

/* ARM7 copy of OSi_DoUnlockByWord; see the declaration in sdk/os_arm7.h. */
int LockWord_ClearIfOwner_Arm7(u16 lockID, OSLockWord* lockp,
                               void (*ctrl)(void), BOOL disableFiq) {
  OSIntrMode intr;

  if (lockID != lockp->ownerID) {
    return LOCK_NOT_OWNER;
  }
  if (disableFiq) {
    intr = Cpu_MaskIrqFiq_Arm7();
  } else {
    intr = Cpu_MaskIrq_Arm7();
  }
  lockp->ownerID = 0;
  if (ctrl != NULL) {
    ctrl();
  }
  lockp->lockFlag = 0;
  if (disableFiq) {
    Cpu_RestoreIrqFiqBits_Arm7(intr);
  } else {
    Cpu_RestoreIrqBit_Arm7(intr);
  }
  return 0;
}
