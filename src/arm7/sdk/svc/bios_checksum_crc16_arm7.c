#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x0e: CRC-16 of a buffer. */
#pragma thumb on
asm u16 Bios_ChecksumCrc16_Arm7(u32 initial, const void *data, u32 size)
{
    swi 0x0e
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* The CRC-16 of size bytes at data, starting from initial: reflected, with
   polynomial 0xa001, no final XOR. The ROM header's two checksums use it
   with initial 0xffff. The BIOS reads halfwords, so data must be
   halfword-aligned and size even; it keeps the CRC in a 32-bit register,
   so initial must fit in 16 bits. Other inputs stop the program. */
u16 Bios_ChecksumCrc16_Arm7(u32 initial, const void* data, u32 size) {
  const u8* bytes = data;
  u32 crc = initial;
  u32 i;
  int bit;

  if ((uintptr_t)data % 2 != 0 || size % 2 != 0 || initial > 0xffff) {
    fprintf(stderr,
            "Bios_ChecksumCrc16_Arm7: %p is not 2-byte aligned, %u is odd "
            "or %#x is wider than 16 bits\n",
            data, size, initial);
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
