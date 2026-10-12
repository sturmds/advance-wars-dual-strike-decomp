#include "sdk/wl.h"

int WifiParam_StoreAllowedChannels(u16 mask) {
  if ((mask & 0x7ffe) == 0) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.unk_00 = mask;
  WIFI_DRIVER_STATE->state.unk_14 |= 4;
  return 0;
}
