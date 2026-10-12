#include "sdk/svc.h"

/* ARM7 BIOS call 0x1a: reads one entry of the BIOS sine table. */
#pragma thumb on
asm int Bios_SineTableEntry_Arm7(int index)
{
    swi 0x1a
    bx lr
}
#pragma thumb reset
