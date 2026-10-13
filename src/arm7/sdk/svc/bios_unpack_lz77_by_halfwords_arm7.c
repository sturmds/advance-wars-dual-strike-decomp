#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x12: LZ77 decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByHalfwords_Arm7(const void *src, void *dst)
{
    swi 0x12
    bx lr
}
#pragma thumb reset
#else
#error "Bios_UnpackLz77ByHalfwords_Arm7: BIOS call 0x12 reads its source through a callback table that no caller in the ROM shows, so C for it would be a guess"
#endif
