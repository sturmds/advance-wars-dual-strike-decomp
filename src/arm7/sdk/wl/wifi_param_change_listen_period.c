#include "sdk/wl.h"

// Wifi registers 0x08e and 0x088 are the listen interval and the listen
// counter (GBATEK).
int WifiParam_ChangeListenPeriod(u16 interval) {
  if (interval < 1 || interval > 255) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.unk_8c = interval;
  WIFI_MAC_REGISTER(0x08e) = interval;
  WIFI_MAC_REGISTER(0x088) = 0;
  return 0;
}
