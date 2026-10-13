#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x0f: reports whether a debugger is attached. */
#pragma thumb on
asm int Bios_RunningOnDevUnit_Arm7(void)
{
    swi 0x0f
    bx lr
}
#pragma thumb reset
#else
#error "Bios_RunningOnDevUnit_Arm7: asking the BIOS whether debugger hardware is present (BIOS call 0x0f) has no portable C equivalent"
#endif
