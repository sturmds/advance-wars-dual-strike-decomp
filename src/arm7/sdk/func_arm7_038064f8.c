#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

void func_arm7_037fb810(int lockID);
void func_arm7_037fdb94(int state);

void func_arm7_038064f8(int lockID, GbaSlotLockRecord *lock)
{
    if (lock->heldBefore == 0) {
        func_arm7_037fb810(lockID);
    }
    func_arm7_037fdb94(lock->savedIrqState);
}
