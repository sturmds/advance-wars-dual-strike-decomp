#include "types.h"

u32 OS_GetLockID(void);
void OS_ReleaseLockID(u16 lockId);
void func_021153f0(u16 a);
void func_021153e0(u16 a);
int CARDi_ReadBackupCore(int a0, int a1, int a2, int a3, int a4, int a5);

int func_0200c1c0(int a0, int a1, int a2) {
  u32 lockId = OS_GetLockID();
  int result;

  func_021153f0(lockId);
  result = CARDi_ReadBackupCore(a0, a1, a2, 0, 0, 0);
  func_021153e0(lockId);
  OS_ReleaseLockID(lockId);
  return result;
}
