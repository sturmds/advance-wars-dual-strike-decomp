#include "sdk/wl.h"

void func_arm7_027c90ec(u16* dst, u16* src);

int WifiCmd_ReportOwnMac(WlRequest* request, WlReply* reply) {
  reply->status = 4;
  func_arm7_027c90ec(&reply->value, WIFI_DRIVER_STATE->macAddress);
  return 0;
}
