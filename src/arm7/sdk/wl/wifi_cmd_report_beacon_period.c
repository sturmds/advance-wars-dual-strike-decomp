#include "sdk/wl.h"

int WifiCmd_ReportBeaconPeriod(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.unk_86;
  return 0;
}
