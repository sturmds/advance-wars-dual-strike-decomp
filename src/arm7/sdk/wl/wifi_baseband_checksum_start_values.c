#include "sdk/wl.h"

void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void *dest);
u16 func_arm7_027c8318(u32 byte, u32 crc);

/*
 * CRC of the 105 baseband start values at firmware 0x64, with only bit 7 of
 * the second value counted. 0x027c8318 updates the CRC by one byte.
 */
u16 WifiBaseband_ChecksumStartValues(void)
{
    u32 offset;
    u32 i;
    u16 crc;
    u32 value;

    offset = 0x64;
    crc = 0;
    value = 0;
    for (i = 0; i < 105; offset++, i++) {
        WifiFlash_ReadRadioBytes(offset, 1, &value);
        if (i == 1) {
            value &= 0x80;
        }
        crc = func_arm7_027c8318(value & 0xff, crc);
    }
    return crc;
}
