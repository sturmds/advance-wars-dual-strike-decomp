#include "sdk/wl.h"

int WifiParam_ChangeBeaconPeriod(int);

int func_arm7_027cdb6c(WlRequest* request, WlReply* reply) {
  reply->status = 1;
  if (WIFI_DRIVER_STATE->state.phase != 1) {
    return 11;
  }
  return WifiParam_ChangeBeaconPeriod(request->unk_10);
}
