#include "sdk/svc.h"

/* BIOS call 0x13: Huffman decompression. */
#pragma thumb on
asm void Bios_UnpackHuffman(const void *src, void *dst)
{
    swi 0x13
    bx lr
}
#pragma thumb reset
