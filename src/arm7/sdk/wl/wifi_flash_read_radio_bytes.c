#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u32 value);

/* Copies size bytes of the wifi block, starting at flash offset offset, from
 * WL's copy to dest. Does nothing when the block was never loaded. */
void WifiFlash_ReadRadioBytes(u32 offset, u32 size, u8 *dest)
{
    const u8 *src;

    if (WIFI_DRIVER_STATE->limits.wifiConfig == NULL) {
        return;
    }
    src = (const u8 *)WIFI_DRIVER_STATE->limits.wifiConfig + offset - WIFI_RADIO_BLOCK_FLASH_OFFSET;
    while (size != 0) {
        WL_WriteByte(dest, WL_ReadByte(src++));
        dest++;
        size--;
    }
}
