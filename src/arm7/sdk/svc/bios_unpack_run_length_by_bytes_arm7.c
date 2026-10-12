#include "sdk/svc.h"

/* ARM7 BIOS call 0x14: run-length decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByBytes_Arm7(const void *src, void *dst)
{
    swi 0x14
    bx lr
}
#pragma thumb reset
