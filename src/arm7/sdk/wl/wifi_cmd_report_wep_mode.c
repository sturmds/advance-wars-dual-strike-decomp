#include "sdk/wl.h"

int WifiCmd_ReportWepMode(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.unk_08;
  return 0;
}
