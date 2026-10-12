#include "sdk/svc.h"

/* BIOS call 0x09: signed division; returns the remainder, which the BIOS
   leaves in r1. */
#pragma thumb on
asm int SVC_DivRem(int numerator, int denominator)
{
    swi 0x09
    mov r0, r1
    bx lr
}
#pragma thumb reset
