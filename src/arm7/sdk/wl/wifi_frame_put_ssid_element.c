#include "sdk/wl.h"

void WL_WriteByte(void* addr, u32 value);
u8 WL_ReadByte(const void* addr);

// Writes the SSID element (id 0) and returns its size.
u32 WifiFrame_PutSsidElement(u8* dest) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  u32 length = 0;
  u32 ssidLength = link->ssidLength;
  u32 i;

  WL_WriteByte(dest, length);
  WL_WriteByte(dest + 1, ssidLength & 0xff);
  length += 2;
  for (i = 0; i < ssidLength; i++) {
    WL_WriteByte(dest + length, WL_ReadByte(&link->ssid[i]));
    length++;
  }
  return length;
}
