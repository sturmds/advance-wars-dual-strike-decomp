#include "sdk/wl.h"

int WifiCmd_SetStationCapacity(WlRequest* request, WlReply* reply) {
  reply->status = 1;
  if (WIFI_DRIVER_STATE->state.mode > 32) {
    return 1;
  }
  request->unk_10++;
  if (request->unk_10 > WIFI_DRIVER_STATE->limits.unk_f4) {
    return 5;
  }
  WIFI_DRIVER_STATE->limits.unk_f6 = request->unk_10;
  return 0;
}
