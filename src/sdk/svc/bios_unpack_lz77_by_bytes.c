#include "sdk/svc.h"

/* BIOS call 0x11: LZ77 decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByBytes(const void *src, void *dst)
{
    swi 0x11
    bx lr
}
#pragma thumb reset
