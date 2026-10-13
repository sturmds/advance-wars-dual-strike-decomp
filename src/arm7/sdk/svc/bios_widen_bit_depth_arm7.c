#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x10: widens packed bit fields. */
#pragma thumb on
asm void Bios_WidenBitDepth_Arm7(const void *src, void *dst, const void *params)
{
    swi 0x10
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stops the program with a message. */
static void WidenFail(const char* what) {
  fprintf(stderr, "Bios_WidenBitDepth_Arm7: %s\n", what);
  abort();
}

/* Reads the source fields from each byte at src, lowest bits first, adds
   the offset to each nonzero field (to every field when bit 31 of the
   offset is set), and packs the results at dstBits each into words at
   dst, lowest bits first.

   The BIOS writes whole words only, so dst must be word-aligned and the
   output a whole number of words. What it does with a width it does not
   list, or a result wider than dstBits, is not known. Such inputs stop
   the program. */
void Bios_WidenBitDepth_Arm7(const void* src, void* dst, const void* params) {
  const UnpackBitsParams* widening = params;
  const u8* in = src;
  u32* out = dst;
  u32 srcBits = widening->srcBits;
  u32 dstBits = widening->dstBits;
  u32 addend = widening->offset & UNPACK_BITS_OFFSET_MASK;
  int addToZero = (widening->offset & UNPACK_BITS_OFFSET_ZEROS) != 0;
  u32 word = 0;
  u32 filled = 0;
  u32 i;
  u32 shift;

  if (srcBits == 0 || srcBits > 8 || (srcBits & (srcBits - 1)) != 0 ||
      dstBits == 0 || dstBits > 32 || (dstBits & (dstBits - 1)) != 0) {
    WidenFail("a width is not one the BIOS lists");
  }
  if ((uintptr_t)dst % 4 != 0 ||
      (u32)widening->srcBytes * 8 / srcBits * dstBits % 32 != 0) {
    WidenFail("dst is not 4-byte aligned or the output is not whole words");
  }
  for (i = 0; i < widening->srcBytes; i++) {
    for (shift = 0; shift < 8; shift += srcBits) {
      u32 field = (in[i] >> shift) & ((1u << srcBits) - 1);
      if (field != 0 || addToZero) {
        field += addend;
      }
      if (dstBits < 32 && field >> dstBits != 0) {
        WidenFail("a result is wider than the output field");
      }
      word |= field << filled;
      filled += dstBits;
      if (filled == 32) {
        *out++ = word;
        word = 0;
        filled = 0;
      }
    }
  }
}
#endif
