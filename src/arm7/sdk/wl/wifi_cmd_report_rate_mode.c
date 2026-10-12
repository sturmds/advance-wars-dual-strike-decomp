#include "sdk/wl.h"

int WifiCmd_ReportRateMode(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.unk_04;
  return 0;
}
