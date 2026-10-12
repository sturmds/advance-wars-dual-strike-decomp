#include "sdk/svc.h"

/* ARM7 BIOS call 0x15: run-length decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByHalfwords_Arm7(const void *src, void *dst)
{
    swi 0x15
    bx lr
}
#pragma thumb reset
