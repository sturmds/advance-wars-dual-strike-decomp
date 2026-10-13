#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x15: run-length decompression, halfword writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByHalfwords_Arm7(const void *src, void *dst)
{
    swi 0x15
    bx lr
}
#pragma thumb reset
#else
#error "Bios_UnpackRunLengthByHalfwords_Arm7: BIOS call 0x15 reads its source through a callback table that no caller in the ROM shows, so C for it would be a guess"
#endif
