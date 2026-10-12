#include "types.h"
#include "sdk/nvram.h"
#include "sdk/spi.h"
#include "sdk/wl.h"

void func_arm7_027d428c(void);

/* Copies the wifi block of the firmware flash header into a new WL buffer.
 * FALSE when its length is out of range or the allocation fails. */
BOOL WifiFlash_FetchRadioBlock(void)
{
    u32 length;

    SPI_Lock(WIFI_DRIVER_STATE->limits.spiLockId);
    func_arm7_027d428c();
    length = 0;
    NVRAM_Read(WIFI_RADIO_BLOCK_FLASH_OFFSET + 2, 2, (u8 *)&length);
    SPI_Unlock(WIFI_DRIVER_STATE->limits.spiLockId);

    if (length < WIFI_RADIO_BLOCK_SIZE_LOW || length > WIFI_RADIO_BLOCK_SIZE_HIGH) {
        return FALSE;
    }
    length += 2;   /* the CRC in front */

    WIFI_DRIVER_STATE->limits.wifiConfig = (WlWifiConfig *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, length);
    if (WIFI_DRIVER_STATE->limits.wifiConfig == NULL) {
        return FALSE;
    }
    /* the data starts after the buffer header */
    WIFI_DRIVER_STATE->limits.wifiConfig = (WlWifiConfig *)((WlBuffer *)WIFI_DRIVER_STATE->limits.wifiConfig + 1);

    SPI_Lock(WIFI_DRIVER_STATE->limits.spiLockId);
    func_arm7_027d428c();
    NVRAM_Read(WIFI_RADIO_BLOCK_FLASH_OFFSET, length, (u8 *)WIFI_DRIVER_STATE->limits.wifiConfig);
    SPI_Unlock(WIFI_DRIVER_STATE->limits.spiLockId);
    return TRUE;
}
