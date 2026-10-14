#include "sdk/wl.h"

extern const WlRegValue gWlRegDefaults[WIFI_MAC_REG_DEFAULTS];

void WifiMac_LoadRegisterTable(void) {
  u32 i;

  for (i = 0; i < WIFI_MAC_REG_DEFAULTS; i++) {
    WIFI_MAC_REGISTER(gWlRegDefaults[i].offset) = gWlRegDefaults[i].value;
  }
}
