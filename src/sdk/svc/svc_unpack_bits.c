#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x10: widens packed bit fields. */
#pragma thumb on
asm void SVC_UnpackBits(const void *src, void *dst, const void *params)
{
    swi 0x10
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void UnpackBitsFail(const char* what) {
  fprintf(stderr, "SVC_UnpackBits: %s\n", what);
  abort();
}

static int IsPowerOfTwoUpTo(u32 width, u32 most) {
  return width != 0 && width <= most && (width & (width - 1)) == 0;
}

/* Reads each input byte's fields from the low bits up. A field that is not
   0, or any field when bit 31 of the offset is set, gets the offset added.
   The results are packed into dst from the low bits up and written a word
   at a time. Assumes every result fits its output field and the output
   fills whole words; the BIOS is not reproduced for other input, so it
   stops the program, as it does for a dst that is not word aligned. */
void SVC_UnpackBits(const void* src, void* dst, const void* params) {
  const UnpackBitsParams* p = params;
  const u8* in = src;
  u32* out = dst;
  u32 offset = p->offset & UNPACK_BITS_OFFSET_MASK;
  u32 fieldMask;
  u32 word = 0;
  u32 wordBits = 0;
  u32 i;
  u32 bit;

  if (!IsPowerOfTwoUpTo(p->srcBits, 8) || !IsPowerOfTwoUpTo(p->dstBits, 32)) {
    UnpackBitsFail("a field width is not 1, 2, 4, 8, 16 or 32");
  }
  fieldMask = (1u << p->srcBits) - 1;
  if ((uintptr_t)dst % 4 != 0) {
    UnpackBitsFail("dst is not word aligned");
  }
  if ((u32)p->srcBytes * 8 / p->srcBits * p->dstBits % 32 != 0) {
    UnpackBitsFail("the output does not end on a word");
  }
  for (i = 0; i < p->srcBytes; i++) {
    for (bit = 0; bit < BITS_PER_BYTE; bit += p->srcBits) {
      u32 value = (in[i] >> bit) & fieldMask;

      if (value != 0 || (p->offset & UNPACK_BITS_OFFSET_ZEROS)) {
        value += offset;
      }
      if (p->dstBits < 32 && value >> p->dstBits != 0) {
        UnpackBitsFail("a value does not fit its output field");
      }
      word |= value << wordBits;
      wordBits += p->dstBits;
      if (wordBits == 32) {
        *out++ = word;
        word = 0;
        wordBits = 0;
      }
    }
  }
}
#endif
