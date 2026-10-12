#include "types.h"
#include "sdk/os_arm7.h"

asm void SlotLock_ReleaseAlias_Arm7(u16 lockID)
{
    ldr r1, =SlotLock_Release_Arm7
    bx r1
}
