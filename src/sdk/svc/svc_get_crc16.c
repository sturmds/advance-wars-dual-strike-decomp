#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x0e: CRC-16 of a buffer. */
#pragma thumb on
asm u16 SVC_GetCRC16(u32 initial, const void *data, u32 size)
{
    swi 0x0e
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* CRC-16 with the bit-reversed polynomial 0xa001, one bit at a time from
   the low end, starting from the low 16 bits of initial. The BIOS reads
   halfwords, so the C takes only a halfword-aligned buffer and an even
   size; other input stops the program. */
u16 SVC_GetCRC16(u32 initial, const void* data, u32 size) {
  const u8* bytes = data;
  u16 crc = (u16)initial;
  u32 i;
  int bit;

  if ((uintptr_t)data % 2 != 0 || size % 2 != 0) {
    fprintf(stderr, "SVC_GetCRC16: %p or size %u is not halfword aligned\n",
            data, (unsigned)size);
    abort();
  }
  for (i = 0; i < size; i++) {
    crc ^= bytes[i];
    for (bit = 0; bit < BITS_PER_BYTE; bit++) {
      crc = (crc & 1) ? (crc >> 1) ^ 0xa001 : crc >> 1;
    }
  }
  return crc;
}
#endif
