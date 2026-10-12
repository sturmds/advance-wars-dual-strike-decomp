#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
u16 Wifi_Crc16FeedByte(u32 byte, u32 crc); /* CRC-16 of one more byte */

/* Checks the length and CRC of the loaded wifi block (WIFI_RADIO_BLOCK_*).
 * With crcs, also stores the stored CRC in the low half and the computed
 * one in the high half. */
u32 WifiFlash_ValidateRadioBlock(u32* crcs) {
  u32 crc;
  u16 storedCrc;
  const u8* src;
  u32 length;

  length = WIFI_DRIVER_STATE->limits.wifiConfig->length;
  if (length < WIFI_RADIO_BLOCK_SIZE_LOW ||
      length > WIFI_RADIO_BLOCK_SIZE_HIGH) {
    return WIFI_RADIO_BLOCK_SIZE_WRONG;
  }

  src = (const u8*)&WIFI_DRIVER_STATE->limits.wifiConfig->length;
  crc = 0;
  while (length != 0) {
    crc = Wifi_Crc16FeedByte(WL_ReadByte(src++) & 0xff, (u16)crc);
    length--;
  }

  storedCrc = WIFI_DRIVER_STATE->limits.wifiConfig->crc;
  if (crcs != NULL) {
    *crcs = storedCrc | (crc << 16);
  }
  return crc !=
         storedCrc; /* WIFI_RADIO_BLOCK_CRC_WRONG or WIFI_RADIO_BLOCK_VALID */
}
