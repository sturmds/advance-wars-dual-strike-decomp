#include "sdk/wl.h"

void WifiStation_RefreshTimeout(u32 index) {
  WIFI_PEER_ARRAY[index].timeout = WIFI_PEER_ARRAY[index].timeoutReload;
}
