#include "types.h"
#include "sdk/os.h"

void func_021153f0(u16 a);
void func_021153e0(u16 a);
int CARDi_VerifyBackupCore(int a0, int a1, int a2, int a3, int a4, int a5);

/* Runs CARDi_VerifyBackupCore between a func_021153f0 / func_021153e0 pair that
 * takes a lock ID from OS_GetLockID, then releases the ID. */
int func_0200c150(int a0, int a1, int a2) {
  int lockId = OS_GetLockID();
  int result;

  func_021153f0(lockId);
  result = CARDi_VerifyBackupCore(a0, a1, a2, 0, 0, 0);
  func_021153e0(lockId);
  OS_ReleaseLockID(lockId);
  return result;
}
