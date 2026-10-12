#include "sdk/svc.h"

/* BIOS call 0x04: halt until a selected interrupt occurs. */
#pragma thumb on
asm void Bios_AwaitIrqFlags(int clearFlags, u32 intrFlags)
{
    swi 0x04
    bx lr
}
#pragma thumb reset
