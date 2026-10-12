#include "sdk/wl.h"

/* CRC-16 with the reflected polynomial 0xa001, one entry per nibble. */
extern const u16 gWlCrc16Table[16];

/* Adds the low byte of data to crc, low nibble first. */
u16 Wifi_Crc16FeedByte(u16 data, u16 crc) {
  u16 entry;

  entry = gWlCrc16Table[crc & 0xf];
  crc = (crc >> 4) & 0xfff;
  crc = crc ^ entry ^ gWlCrc16Table[data & 0xf];
  entry = gWlCrc16Table[crc & 0xf];
  crc = (crc >> 4) & 0xfff;
  crc = crc ^ entry ^ gWlCrc16Table[(data >> 4) & 0xf];
  return crc;
}
