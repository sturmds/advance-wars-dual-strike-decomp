#include "sdk/wl.h"

int WifiCmd_ReportAssocListen(WlRequest* request, WlReply* reply) {
  reply->status = 2;
  reply->value = WIFI_DRIVER_STATE->state.unk_88;
  return 0;
}
