#include "types.h"
#include "sdk/os_arm7.h"

#ifdef __MWERKS__
asm void SlotLock_ReleaseAlias_Arm7(u16 lockID)
{
    ldr r1, =SlotLock_Release_Arm7
    bx r1
}
#else
/* The assembly jumps to SlotLock_Release_Arm7, so callers get its s32 in
   r0; the prototype says void, so the C ignores it. */
void SlotLock_ReleaseAlias_Arm7(u16 lockID) { SlotLock_Release_Arm7(lockID); }
#endif
