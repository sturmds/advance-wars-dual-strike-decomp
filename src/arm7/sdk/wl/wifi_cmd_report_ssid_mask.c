#include "sdk/wl.h"

int WifiCmd_ReportSsidMask(WlRequest* request, WlReply* reply) {
  u32 i;
  u16* dst;
  u16* src;

  reply->status = 17;
  dst = &reply->value;
  src = WIFI_DRIVER_STATE->state.words_58;
  for (i = 0; i < ARRAY_COUNT(WIFI_DRIVER_STATE->state.words_58); i++) {
    *dst++ = *src++;
  }
  return 0;
}
