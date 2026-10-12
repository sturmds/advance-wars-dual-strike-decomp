#include "sdk/wl.h"

int WifiParam_SetSsidHidden(u32 enable) {
  if (enable > 1) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.flags.raw =
      (WIFI_DRIVER_STATE->state.flags.raw & ~1) | (enable & 1);
  return 0;
}
