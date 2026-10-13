#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x1b: reads one entry of the BIOS pitch table. */
#pragma thumb on
asm int Bios_PitchTableEntry_Arm7(int index)
{
    swi 0x1b
    bx lr
}
#pragma thumb reset
#else
#error "Bios_PitchTableEntry_Arm7: reading the pitch table in the BIOS ROM (BIOS call 0x1b) has no portable C equivalent"
#endif
