#include "sdk/svc.h"

/* BIOS call 0x10: widens packed bit fields. */
#pragma thumb on
asm void SVC_UnpackBits(const void *src, void *dst, const void *params)
{
    swi 0x10
    bx lr
}
#pragma thumb reset
