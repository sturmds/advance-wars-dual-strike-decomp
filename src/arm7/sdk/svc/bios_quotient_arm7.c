#include "sdk/svc.h"

/* ARM7 BIOS call 0x09: signed division; returns the quotient. */
#pragma thumb on
asm int Bios_Quotient_Arm7(int numerator, int denominator)
{
    swi 0x09
    bx lr
}
#pragma thumb reset
