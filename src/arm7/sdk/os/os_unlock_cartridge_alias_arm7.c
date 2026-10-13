#include "types.h"
#include "sdk/os_arm7.h"

#ifdef __MWERKS__
asm s32 SlotLock_ReleaseAlias_Arm7(u16 lockID)
{
    ldr r1, =SlotLock_Release_Arm7
    bx r1
}
#else
/* The assembly jumps to SlotLock_Release_Arm7 and returns its result. */
s32 SlotLock_ReleaseAlias_Arm7(u16 lockID) {
  return SlotLock_Release_Arm7(lockID);
}
#endif
