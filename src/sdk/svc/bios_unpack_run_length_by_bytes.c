#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x14: run-length decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByBytes(const void *src, void *dst)
{
    swi 0x14
    bx lr
}
#pragma thumb reset
#else
#include <stdio.h>
#include <stdlib.h>

#define RUN_LENGTH_TYPE 3

static void RunLengthFail(const char* what) {
  fprintf(stderr, "Bios_UnpackRunLengthByBytes: %s\n", what);
  abort();
}

/* src starts with a word: the type (3) in bits 4-7 and the unpacked size in
   bits 8-31. Then each flag byte starts a run: with bit 7 set, the next
   byte written (flag & 0x7f) + 3 times; with it clear, the next
   (flag & 0x7f) + 1 bytes as they are. Assumes well-formed data: the type
   is 3, the size is not zero and the last run ends exactly at the size.
   The BIOS is not reproduced for other input, so it stops the program. */
void Bios_UnpackRunLengthByBytes(const void* src, void* dst) {
  const u8* in = src;
  u8* out = dst;
  u32 header = in[0] | (in[1] << 8) | (in[2] << 16) | ((u32)in[3] << 24);
  u32 size = header >> 8;
  u32 done = 0;

  if (((header >> 4) & 0xf) != RUN_LENGTH_TYPE) {
    RunLengthFail("the header does not say run-length");
  }
  if (size == 0) {
    RunLengthFail("the unpacked size is 0");
  }
  in += 4;
  while (done < size) {
    u8 flag = *in++;
    u32 length = (flag & 0x80) ? (flag & 0x7f) + 3 : (flag & 0x7f) + 1;

    if (length > size - done) {
      RunLengthFail("a run goes past the unpacked size");
    }
    if (flag & 0x80) {
      u8 value = *in++;
      while (length-- != 0) {
        out[done++] = value;
      }
    } else {
      while (length-- != 0) {
        out[done++] = *in++;
      }
    }
  }
}
#endif
