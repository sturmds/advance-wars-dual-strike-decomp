#include "sdk/svc.h"

/* ARM7 BIOS call 0x1c: reads one entry of the BIOS volume table. */
#pragma thumb on
asm int Bios_VolumeTableEntry_Arm7(int index)
{
    swi 0x1c
    bx lr
}
#pragma thumb reset
