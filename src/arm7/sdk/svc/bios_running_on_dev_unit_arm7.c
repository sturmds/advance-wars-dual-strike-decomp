#include "sdk/svc.h"

/* ARM7 BIOS call 0x0f: reports whether a debugger is attached. */
#pragma thumb on
asm int Bios_RunningOnDevUnit_Arm7(void)
{
    swi 0x0f
    bx lr
}
#pragma thumb reset
