#include "sdk/svc.h"

/* ARM7 BIOS call 0x0c: word copy and fill, 32 bytes at a time. */
#pragma thumb on
asm void Bios_FastFillOrCopy_Arm7(const void *src, void *dst, u32 control)
{
    swi 0x0c
    bx lr
}
#pragma thumb reset
