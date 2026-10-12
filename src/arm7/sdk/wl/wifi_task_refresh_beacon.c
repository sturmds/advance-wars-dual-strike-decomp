#include "sdk/wl.h"

void WifiBeacon_CopyGameInfo(void);

// Task 8: puts changed game info into the beacon, then, with option bit 6
// set, sends host message 0x8c.
void WifiTask_RefreshBeacon(void) {
  if (WIFI_DRIVER_STATE->state.gameInfoChanged) {
    WifiBeacon_CopyGameInfo();
  }
  if (WIFI_DRIVER_STATE->state.flags.bit6) {
    WifiHost_TellBeaconSent();
  }
}
