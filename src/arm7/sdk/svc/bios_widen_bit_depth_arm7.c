#include "sdk/svc.h"

/* ARM7 BIOS call 0x10: widens packed bit fields. */
#pragma thumb on
asm void Bios_WidenBitDepth_Arm7(const void *src, void *dst, const void *params)
{
    swi 0x10
    bx lr
}
#pragma thumb reset
