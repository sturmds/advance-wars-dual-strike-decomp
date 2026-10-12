#include "sdk/wl.h"

int WifiParam_SetSsidHidden(int);

int func_arm7_027cded8(WlRequest* request, WlReply* reply) {
  reply->status = 1;
  if (WIFI_DRIVER_STATE->state.mode > 32) {
    return 1;
  }
  return WifiParam_SetSsidHidden(request->unk_10);
}
