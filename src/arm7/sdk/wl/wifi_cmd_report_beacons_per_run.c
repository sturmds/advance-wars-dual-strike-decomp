#include "sdk/wl.h"

int WifiCmd_ReportBeaconsPerRun(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.unk_96;
  return 0;
}
