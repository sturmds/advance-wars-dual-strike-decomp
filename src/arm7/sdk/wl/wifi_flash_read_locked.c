#include "types.h"
#include "sdk/nvram.h"
#include "sdk/spi.h"
#include "sdk/wl.h"

void WifiFlash_AwaitIdle(void);

/* Reads size bytes of firmware flash at address, holding the SPI bus. */
void WifiFlash_ReadLocked(u32 address, u32 size, u8 *dest)
{
    SPI_Lock(WIFI_DRIVER_STATE->limits.spiLockId);
    WifiFlash_AwaitIdle();
    NVRAM_Read(address, size, dest);
    SPI_Unlock(WIFI_DRIVER_STATE->limits.spiLockId);
}
