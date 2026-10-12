#include "sdk/svc.h"

/* ARM7 BIOS call 0x0e: CRC-16 of a buffer. */
#pragma thumb on
asm u16 Bios_ChecksumCrc16_Arm7(u32 initial, const void *data, u32 size)
{
    swi 0x0e
    bx lr
}
#pragma thumb reset
