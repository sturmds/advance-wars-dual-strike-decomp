#include "sdk/wl.h"

int WifiCmd_ReportProbeNeedsSsid(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.flags.bit1;
  return 0;
}
