#include "sdk/wl.h"

extern const WlRegValue gWlRegDefaults[25];

void WifiMac_LoadRegisterTable(void) {
  u32 i;

  for (i = 0; i < 25; i++) {
    WIFI_MAC_REGISTER(gWlRegDefaults[i].offset) = gWlRegDefaults[i].value;
  }
}
