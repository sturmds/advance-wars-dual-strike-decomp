#include "sdk/svc.h"

/* BIOS call 0x09: signed division; returns the quotient. */
#pragma thumb on
asm int SVC_Div(int numerator, int denominator)
{
    swi 0x09
    bx lr
}
#pragma thumb reset
