#include "types.h"
#include "sdk/os.h"

void func_021153f0(u16 a);
void func_021153e0(u16 a);
int CARDi_WriteBackupCore(int a0, int a1, int a2, int a3, int a4, int a5);

int func_0200c230(int a0, int a1, int a2) {
  int lockId = OS_GetLockID();
  int result;

  func_021153f0(lockId);
  result = CARDi_WriteBackupCore(a0, a1, a2, 0, 0, 0);
  func_021153e0(lockId);
  OS_ReleaseLockID(lockId);
  return result;
}
