#include "sdk/wl.h"

int WifiCmd_ReportBeaconForwarding(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  if (WIFI_DRIVER_STATE->state.mode < 16) {
    return 1;
  }
  reply->value = WIFI_DRIVER_STATE->state.flags.bit6;
  return 0;
}
