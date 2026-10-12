#include "sdk/svc.h"

/* ARM7 BIOS call 0x1b: reads one entry of the BIOS pitch table. */
#pragma thumb on
asm int Bios_PitchTableEntry_Arm7(int index)
{
    swi 0x1b
    bx lr
}
#pragma thumb reset
