#include "sdk/svc.h"

/* ARM7 BIOS call 0x12: LZ77 decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByHalfwords_Arm7(const void *src, void *dst)
{
    swi 0x12
    bx lr
}
#pragma thumb reset
