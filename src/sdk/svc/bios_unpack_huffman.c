#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x13: Huffman decompression. */
#pragma thumb on
asm void Bios_UnpackHuffman(const void *src, void *dst)
{
    swi 0x13
    bx lr
}
#pragma thumb reset
#else
/* On the DS this BIOS call reads its input through a table of callbacks in
   r3 with a user argument in r2. The prototype declares neither, and
   nothing in the ROM calls this stub, so C cannot know what they hold. */
#error "Bios_UnpackHuffman: BIOS call 0x13 reads its input through callbacks in r2 and r3 that the prototype does not declare"
#endif
