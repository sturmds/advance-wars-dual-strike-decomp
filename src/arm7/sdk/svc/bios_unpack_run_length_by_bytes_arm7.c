#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x14: run-length decompression, byte writes. */
#pragma thumb on
asm void Bios_UnpackRunLengthByBytes_Arm7(const void *src, void *dst)
{
    swi 0x14
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stops the program with a message about the stream at src. */
static void RunLengthFail(const void* src, const char* what) {
  fprintf(stderr, "Bios_UnpackRunLengthByBytes_Arm7: stream at %p: %s\n",
          src, what);
  abort();
}

/* Unpacks the run-length stream at src to dst, in the format the ROM's own
   unpacker reads (Unpack_RunLengthByBytes): a 4-byte header, type 3 in
   bits 4-7 and the unpacked size in bits 8-31, then blocks. A block's
   control byte holds a count in its low 7 bits; with bit 7 clear,
   count + 1 literal bytes follow, with it set, one byte follows and is
   written count + 3 times.

   The BIOS reads the header as a word, so src must be word-aligned. A
   misaligned src, another type, or a block that runs past the size stops
   the program: what the BIOS does then is not known. */
void Bios_UnpackRunLengthByBytes_Arm7(const void* src, void* dst) {
  const u8* in = src;
  u8* out = dst;
  u32 header;
  u32 size;
  u32 written = 0;
  u32 length;
  u8 control;

  if ((uintptr_t)src % 4 != 0) {
    RunLengthFail(src, "not 4-byte aligned");
  }
  header = in[0] | in[1] << 8 | in[2] << 16 | (u32)in[3] << 24;
  if ((header >> 4 & 0xf) != 3) {
    RunLengthFail(src, "type is not run-length");
  }
  size = header >> 8;
  in += 4;
  while (written < size) {
    control = *in++;
    length = (control & 0x7f) + ((control & 0x80) ? 3 : 1);
    if (length > size - written) {
      RunLengthFail(src, "a block runs past the unpacked size");
    }
    if (control & 0x80) {
      for (; length != 0; length--) {
        out[written++] = *in;
      }
      in++;
    } else {
      for (; length != 0; length--) {
        out[written++] = *in++;
      }
    }
  }
}
#endif
