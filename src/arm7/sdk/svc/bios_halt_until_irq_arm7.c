#include "sdk/svc.h"

/* ARM7 BIOS call 0x06: halts the CPU until any enabled interrupt occurs. */
#pragma thumb on
asm void Bios_HaltUntilIrq_Arm7(void)
{
    swi 0x06
    bx lr
}
#pragma thumb reset
