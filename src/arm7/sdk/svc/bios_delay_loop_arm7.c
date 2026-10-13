#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x03: busy-waits for count loop iterations. */
#pragma thumb on
asm void Bios_DelayLoop_Arm7(int count)
{
    swi 0x03
    bx lr
}
#pragma thumb reset
#else
#error "Bios_DelayLoop_Arm7: waiting a fixed number of CPU cycles (BIOS call 0x03) has no portable C equivalent"
#endif
