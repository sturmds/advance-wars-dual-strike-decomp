#include "sdk/wl.h"

int WifiCmd_ReportChannelInfo(WlRequest* request, WlReply* reply) {
  reply->status = 3;
  reply->value = WIFI_DRIVER_STATE->state.unk_00;
  reply->words[0] = WIFI_DRIVER_STATE->state.unk_92;
  return 0;
}
