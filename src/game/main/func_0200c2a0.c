#include "types.h"
#include "sdk/os.h"

void func_021153f0(u16 a);
void func_021153e0(u16 a);
void CARD_IdentifyBackup(int a);
int CARDi_ReadBackupCore(int a0, void* a1, int a2, int a3, int a4, int a5);

int func_0200c2a0(void) {
  int lockId = OS_GetLockID();
  int result;
  u32 buffer;

  CARD_IdentifyBackup(0x1202);
  func_021153f0(lockId);
  result = CARDi_ReadBackupCore(0, &buffer, 4, 0, 0, 0);
  func_021153e0(lockId);
  OS_ReleaseLockID(lockId);
  return result;
}
