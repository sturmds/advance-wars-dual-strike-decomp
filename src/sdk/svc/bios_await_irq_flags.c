#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x04: halt until a selected interrupt occurs. */
#pragma thumb on
asm void Bios_AwaitIrqFlags(int clearFlags, u32 intrFlags)
{
    swi 0x04
    bx lr
}
#pragma thumb reset
#else
#error "Bios_AwaitIrqFlags: halting the CPU until an interrupt (BIOS call 0x04) has no portable C equivalent"
#endif
