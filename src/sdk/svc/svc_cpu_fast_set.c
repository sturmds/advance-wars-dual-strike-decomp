#include "sdk/svc.h"

/* BIOS call 0x0c: word copy and fill, 32 bytes at a time. */
#pragma thumb on
asm void SVC_CpuFastSet(const void *src, void *dst, u32 control)
{
    swi 0x0c
    bx lr
}
#pragma thumb reset
