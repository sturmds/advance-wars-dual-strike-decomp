#include "sdk/wl.h"

int WifiCmd_ReadBasebandPair(WlRequest* request, WlReply* reply) {
  reply->status = 3;
  reply->value = WifiBaseband_Get(0x13);
  reply->words[0] = WifiBaseband_Get(0x35);
  return 0;
}
