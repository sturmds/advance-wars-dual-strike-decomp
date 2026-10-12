#include "sdk/svc.h"

/* ARM7 BIOS call 0x07: puts the system to sleep until an enabled wake-up
   interrupt occurs. */
#pragma thumb on
asm void Bios_EnterSleepMode_Arm7(void)
{
    swi 0x07
    bx lr
}
#pragma thumb reset
