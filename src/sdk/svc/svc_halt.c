#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x06: halts the CPU until any enabled interrupt occurs. */
#pragma thumb on
asm void SVC_Halt(void)
{
    swi 0x06
    bx lr
}
#pragma thumb reset
#else
#error "SVC_Halt: halting the CPU until an interrupt (BIOS call 0x06) has no portable C equivalent"
#endif
