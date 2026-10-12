#include "sdk/svc.h"

/* ARM7 BIOS call 0x11: LZ77 decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByBytes_Arm7(const void *src, void *dst)
{
    swi 0x11
    bx lr
}
#pragma thumb reset
