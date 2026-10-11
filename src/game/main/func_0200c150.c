#include "types.h"

u32 OS_GetLockID(void);
void OS_ReleaseLockID(u16 lockId);
void func_021153f0(u16 a);
void func_021153e0(u16 a);
int func_02115ad4(int a0, int a1, int a2, int a3, int a4, int a5);

/* Runs func_02115ad4 between a func_021153f0 / func_021153e0 pair that
 * takes a lock ID from OS_GetLockID, then releases the ID. */
int func_0200c150(int a0, int a1, int a2)
{
    u32 lockId = OS_GetLockID();
    int result;

    func_021153f0(lockId);
    result = func_02115ad4(a0, a1, a2, 0, 0, 0);
    func_021153e0(lockId);
    OS_ReleaseLockID(lockId);
    return result;
}
