#include "sdk/wl.h"

void Wifi_SwitchMode(int mode); /* switches WlLink.mode */

int WifiCmd_PowerDownMode(WlRequest* request, WlReply* reply) {
  reply->status = 1;
  if (WIFI_DRIVER_STATE->state.mode == 0 ||
      WIFI_DRIVER_STATE->state.mode == WIFI_MODE_RADIO_ON) {
    Wifi_SwitchMode(0);
    return 0;
  }
  return 1;
}
