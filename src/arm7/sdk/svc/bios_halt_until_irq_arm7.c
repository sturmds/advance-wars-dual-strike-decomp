#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x06: halts the CPU until any enabled interrupt occurs. */
#pragma thumb on
asm void Bios_HaltUntilIrq_Arm7(void)
{
    swi 0x06
    bx lr
}
#pragma thumb reset
#else
#error "Bios_HaltUntilIrq_Arm7: halting the CPU until an interrupt (BIOS call 0x06) has no portable C equivalent"
#endif
