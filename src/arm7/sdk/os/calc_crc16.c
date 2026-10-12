#include "types.h"

extern u16 gCrc16NibbleTable[16]; /* CRC-16 (polynomial 0xa001) per nibble */

/*
 * Updates `crc` over `size` bytes (rounded down to whole halfwords) of
 * `data`, four bits at a time, low nibble first. The ARM7 startup uses it
 * to check the two copies of the firmware user settings.
 */
u16 CalcCrc16(u16 crc, const u16* data, u32 size) {
  const u16* end = (const u16*)((const u8*)data + (size & ~1));
  u16 value;
  u32 shift = 0;

  while (data < end) {
    if (shift == 0) {
      value = *data;
    }
    /* needed to match: the cast gives lsl #12 then lsr #16, not asr #4 */
    crc = (u16)(crc >> 4) ^ gCrc16NibbleTable[crc & 0xf] ^
          gCrc16NibbleTable[(value >> shift) & 0xf];
    shift += 4;
    if (shift >= 16) {
      shift = 0;
      data++;
    }
  }
  return crc;
}
