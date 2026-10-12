#include "types.h"
#include "sdk/os_arm7.h"

s32 SlotLock_Release_Arm7(u16 lockID) {
  return LockWord_ClearIfOwner_Arm7(lockID, SLOTLOCK_WORD,
                                    SlotLock_OnReleaseNoop_Arm7, TRUE);
}
