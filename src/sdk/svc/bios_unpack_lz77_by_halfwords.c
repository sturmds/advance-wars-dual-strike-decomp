#include "sdk/svc.h"

/* BIOS call 0x12: LZ77 decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByHalfwords(const void *src, void *dst)
{
    swi 0x12
    bx lr
}
#pragma thumb reset
