#include "sdk/os_arm7.h"

/*
 * Takes a lock word, waiting until it is free: retries
 * LockWord_SwapOnce_Arm7 while it returns another owner's lock flag. Returns the
 * last lock flag (0 once the lock is ours).
 */
s32 LockWord_SpinAcquire_Arm7(u16 lockID, OSLockWord* lockp, void (*ctrl)(void),
                              BOOL disableFiq) {
  s32 lockFlag;

  while ((lockFlag = LockWord_SwapOnce_Arm7(lockID, lockp, ctrl, disableFiq)) >
         0) {
    LockWord_PauseBetweenTries_Arm7();
  }
  return lockFlag;
}
