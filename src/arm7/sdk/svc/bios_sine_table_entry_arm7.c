#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x1a: reads one entry of the BIOS sine table. */
#pragma thumb on
asm int Bios_SineTableEntry_Arm7(int index)
{
    swi 0x1a
    bx lr
}
#pragma thumb reset
#else
#error "Bios_SineTableEntry_Arm7: reading the sine table in the BIOS ROM (BIOS call 0x1a) has no portable C equivalent"
#endif
