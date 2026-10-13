#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x07: puts the system to sleep until an enabled wake-up
   interrupt occurs. */
#pragma thumb on
asm void Bios_EnterSleepMode_Arm7(void)
{
    swi 0x07
    bx lr
}
#pragma thumb reset
#else
#error "Bios_EnterSleepMode_Arm7: putting the system to sleep until a wake-up interrupt (BIOS call 0x07) has no portable C equivalent"
#endif
