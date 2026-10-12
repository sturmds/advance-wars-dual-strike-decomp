#include "sdk/svc.h"

/* ARM7 BIOS call 0x09: signed division; returns the remainder, which the
   BIOS leaves in r1. */
#pragma thumb on
asm int Bios_Remainder_Arm7(int numerator, int denominator)
{
    swi 0x09
    mov r0, r1
    bx lr
}
#pragma thumb reset
