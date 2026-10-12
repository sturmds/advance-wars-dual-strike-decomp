#include "types.h"
#include "sdk/os_arm7.h"

s32 SlotLock_TryOnce_Arm7(u16 lockID) {
  return LockWord_SwapOnce_Arm7(lockID, SLOTLOCK_WORD,
                                SlotLock_OnAcquireNoop_Arm7, TRUE);
}
