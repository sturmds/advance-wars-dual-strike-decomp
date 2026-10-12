#include "sdk/wl.h"

int WifiParam_StoreWepMode(u32 value) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);

  if (value > 3) {
    return 5;
  }
  work->state.unk_08 = value;
  if (value == 0) {
    link->flags_7c &= ~0x10;
    link->flags_8a &= ~0x4000;
  } else {
    link->flags_7c |= 0x10;
    link->flags_8a |= 0x4000;
  }
  if (link->mode == 64) {
    if (value == 1) {
      WIFI_DRIVER_STATE->state.txFrame->unk_2e = link->flags_7c;
    }
  }
  if (value == 0) {
    value = 1;
  }
  WIFI_MAC_MODE_AND_WEP = (WIFI_MAC_MODE_AND_WEP & 0xffc7) | (value << 3);
  return 0;
}
