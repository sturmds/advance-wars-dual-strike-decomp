#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x11: LZ77 decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByBytes_Arm7(const void *src, void *dst)
{
    swi 0x11
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stops the program with a message about the stream at src. */
static void Lz77Fail(const void* src, const char* what) {
  fprintf(stderr, "Bios_UnpackLz77ByBytes_Arm7: stream at %p: %s\n", src,
          what);
  abort();
}

/* Unpacks the LZ77 stream at src to dst, in the format the ROM's own
   unpackers read (MI_UncompressLZ8): a 4-byte header, type 1 in bits 4-7
   and the unpacked size in bits 8-31, then flag bytes. Each flag byte
   covers the next eight items, most significant bit first: 0 for a literal
   byte, 1 for two bytes that copy 3 + (top 4 bits) bytes from
   1 + (other 12 bits) bytes back in the output.

   The BIOS reads the header as a word, so src must be word-aligned. A
   misaligned src, another type, or a copy that runs past the size or
   reaches back before dst stops the program: what the BIOS does then is
   not known. */
void Bios_UnpackLz77ByBytes_Arm7(const void* src, void* dst) {
  const u8* in = src;
  u8* out = dst;
  u32 header;
  u32 size;
  u32 written = 0;
  u8 flags;
  int item;

  if ((uintptr_t)src % 4 != 0) {
    Lz77Fail(src, "not 4-byte aligned");
  }
  header = in[0] | in[1] << 8 | in[2] << 16 | (u32)in[3] << 24;
  if ((header >> 4 & 0xf) != 1) {
    Lz77Fail(src, "type is not LZ77");
  }
  size = header >> 8;
  in += 4;
  while (written < size) {
    flags = *in++;
    for (item = 0; item < 8 && written < size; item++, flags <<= 1) {
      if (flags & 0x80) {
        u32 length = (in[0] >> 4) + 3;
        u32 distance = ((in[0] & 0xf) << 8 | in[1]) + 1;
        in += 2;
        if (distance > written || length > size - written) {
          Lz77Fail(src, "a copy reaches outside the output");
        }
        for (; length != 0; length--, written++) {
          out[written] = out[written - distance];
        }
      } else {
        out[written++] = *in++;
      }
    }
  }
}
#endif
