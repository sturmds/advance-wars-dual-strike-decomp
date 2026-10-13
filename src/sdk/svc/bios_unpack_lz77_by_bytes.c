#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x11: LZ77 decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackLz77ByBytes(const void *src, void *dst)
{
    swi 0x11
    bx lr
}
#pragma thumb reset
#else
#include <stdio.h>
#include <stdlib.h>

#define LZ77_TYPE 1

static void Lz77Fail(const char* what) {
  fprintf(stderr, "Bios_UnpackLz77ByBytes: %s\n", what);
  abort();
}

/* src starts with a word: the type (1) in bits 4-7 and the unpacked size in
   bits 8-31. Then each flag byte covers the next eight blocks, highest bit
   first: a clear bit is one literal byte, a set bit is two bytes that copy
   (b0 >> 4) + 3 bytes from (((b0 & 0xf) << 8) | b1) + 1 bytes back in dst.
   Assumes well-formed data: the type is 1, the size is not zero, no copy
   reaches before dst and the last block ends exactly at the size. The BIOS
   is not reproduced for other input, so it stops the program. */
void Bios_UnpackLz77ByBytes(const void* src, void* dst) {
  const u8* in = src;
  u8* out = dst;
  u32 header = in[0] | (in[1] << 8) | (in[2] << 16) | ((u32)in[3] << 24);
  u32 size = header >> 8;
  u32 done = 0;

  if (((header >> 4) & 0xf) != LZ77_TYPE) {
    Lz77Fail("the header does not say LZ77");
  }
  if (size == 0) {
    Lz77Fail("the unpacked size is 0");
  }
  in += 4;
  while (done < size) {
    u8 flags = *in++;
    int block;

    for (block = 0; block < 8 && done < size; block++, flags <<= 1) {
      if (flags & 0x80) {
        u32 length = (in[0] >> 4) + 3;
        u32 distance = (((in[0] & 0xf) << 8) | in[1]) + 1;

        in += 2;
        if (distance > done) {
          Lz77Fail("a copy reaches before dst");
        }
        if (length > size - done) {
          Lz77Fail("a copy runs past the unpacked size");
        }
        while (length-- != 0) {
          out[done] = out[done - distance];
          done++;
        }
      } else {
        out[done++] = *in++;
      }
    }
  }
}
#endif
