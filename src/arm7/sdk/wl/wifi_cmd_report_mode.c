#include "sdk/wl.h"

int WifiCmd_ReportMode(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.mode;
  return 0;
}
