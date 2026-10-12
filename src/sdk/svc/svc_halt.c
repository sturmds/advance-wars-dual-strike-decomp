#include "sdk/svc.h"

/* BIOS call 0x06: halts the CPU until any enabled interrupt occurs. */
#pragma thumb on
asm void SVC_Halt(void)
{
    swi 0x06
    bx lr
}
#pragma thumb reset
