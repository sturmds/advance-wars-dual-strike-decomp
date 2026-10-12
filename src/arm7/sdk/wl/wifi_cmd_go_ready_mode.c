#include "sdk/wl.h"

void Wifi_SwitchMode(int mode); /* switches WlLink.mode */

int WifiCmd_GoReadyMode(WlRequest* request, WlReply* reply) {
  reply->status = 1;
  if (WIFI_DRIVER_STATE->state.mode == WIFI_MODE_RADIO_ON ||
      (WIFI_DRIVER_STATE->state.mode == 32 &&
       WIFI_DRIVER_STATE->state.unk_2a == 0)) {
    Wifi_SwitchMode(32);
    return 0;
  }
  return 1;
}
