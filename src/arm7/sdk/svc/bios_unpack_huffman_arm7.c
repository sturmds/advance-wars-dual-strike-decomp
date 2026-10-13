#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x13: Huffman decompression. */
#pragma thumb on
asm void Bios_UnpackHuffman_Arm7(const void *src, void *dst)
{
    swi 0x13
    bx lr
}
#pragma thumb reset
#else
#error "Bios_UnpackHuffman_Arm7: BIOS call 0x13 reads its source through a callback table that no caller in the ROM shows, so C for it would be a guess"
#endif
