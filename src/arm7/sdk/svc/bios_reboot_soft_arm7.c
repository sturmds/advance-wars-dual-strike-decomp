#include "sdk/svc.h"

/* ARM7 BIOS call 0x00: restarts the system. */
#pragma thumb on
asm void Bios_RebootSoft_Arm7(void)
{
    swi 0x00
    bx lr
}
#pragma thumb reset
