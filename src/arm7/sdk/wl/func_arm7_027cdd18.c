#include "sdk/wl.h"

int func_arm7_027c933c(u32 value);

// Index value that selects every station from 1 up.
#define WIFI_EVERY_STATION_INDEX 0xffff

int func_arm7_027cdd18(WlRequest* request, WlReply* reply) {
  WlStation* stations = WIFI_PEER_ARRAY;
  u32 i;

  reply->status = 1;
  if (request->unk_10 >= WIFI_DRIVER_STATE->limits.unk_f6 &&
      request->unk_10 != WIFI_EVERY_STATION_INDEX) {
    return 5;
  }
  if (request->unk_14 > 63 && request->unk_14 != 0xffff) {
    return 5;
  }
  if (request->unk_10 == WIFI_EVERY_STATION_INDEX) {
    for (i = 1; i < WIFI_DRIVER_STATE->limits.unk_f6; i++) {
      stations[i].timeoutReload = request->unk_12;
      if (stations[i].timeout != 0) {
        stations[i].timeout = request->unk_12;
      }
    }
  } else if (request->unk_10 != 0) {
    stations[request->unk_10].timeoutReload = request->unk_12;
    if (stations[request->unk_10].timeout != 0) {
      stations[request->unk_10].timeout = request->unk_12;
    }
  }
  if (request->unk_14 != 0) {
    func_arm7_027c933c(request->unk_14);
  }
  return 0;
}
