#include "sdk/wl.h"

int WifiParam_SetProbeNeedsSsid(u32 enable) {
  if (enable > 1) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.flags.raw =
      (WIFI_DRIVER_STATE->state.flags.raw & ~2) | ((enable & 1) << 1);
  return 0;
}
