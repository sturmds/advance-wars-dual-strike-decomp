#include "sdk/svc.h"

/* ARM7 BIOS call 0x0b: halfword or word copy and fill. */
#pragma thumb on
asm void Bios_FillOrCopy_Arm7(const void *src, void *dst, u32 control)
{
    swi 0x0b
    bx lr
}
#pragma thumb reset
