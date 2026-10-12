#include "types.h"
#include "sdk/os_arm7.h"

u16 LockWord_OwnerId_Arm7(OSLockWord *lock)
{
    return lock->ownerID;
}
