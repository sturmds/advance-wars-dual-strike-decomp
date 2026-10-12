#include "sdk/svc.h"

/* BIOS call 0x15: run-length decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByHalfwords(const void *src, void *dst)
{
    swi 0x15
    bx lr
}
#pragma thumb reset
