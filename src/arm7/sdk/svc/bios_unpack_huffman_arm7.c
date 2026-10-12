#include "sdk/svc.h"

/* ARM7 BIOS call 0x13: Huffman decompression. */
#pragma thumb on
asm void Bios_UnpackHuffman_Arm7(const void *src, void *dst)
{
    swi 0x13
    bx lr
}
#pragma thumb reset
