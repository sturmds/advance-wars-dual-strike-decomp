#include "sdk/svc.h"

/* BIOS call 0x0b: halfword or word copy and fill. */
#pragma thumb on
asm void SVC_CpuSet(const void *src, void *dst, u32 control)
{
    swi 0x0b
    bx lr
}
#pragma thumb reset
