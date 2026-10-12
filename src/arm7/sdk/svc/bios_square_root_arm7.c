#include "sdk/svc.h"

/* ARM7 BIOS call 0x0d: integer square root. */
#pragma thumb on
asm u32 Bios_SquareRoot_Arm7(u32 value)
{
    swi 0x0d
    bx lr
}
#pragma thumb reset
