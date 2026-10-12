#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
void WL_WriteByte(void* addr, u32 value);

int WifiParam_StoreSsid(u32 length, const u8* src) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  u32 i;
  BOOL updateFrame = FALSE;
  u8* dst;

  if (length > 32) {
    return 5;
  }
  if (link->mode == 64 && work->state.phase == 1) {
    if (link->ssidLength != length) {
      return 6;
    }
    if (link->ssidOffset != 0) {
      updateFrame = TRUE;
    }
  }
  for (i = 0; i < length; i++) {
    WL_WriteByte(link->ssid + i, WL_ReadByte(src));
    src++;
  }
  for (; i < 32; i++) {
    WL_WriteByte(link->ssid + i, 0);
  }
  link->ssidLength = length;
  if (updateFrame) {
    // + 2 skips the element id and length bytes.
    dst = WIFI_DRIVER_STATE->state.txFrame->payload + 2 + link->ssidOffset;
    for (i = 0; i < length; i++) {
      WL_WriteByte(dst + i, WL_ReadByte(link->ssid + i));
    }
  }
  return 0;
}
