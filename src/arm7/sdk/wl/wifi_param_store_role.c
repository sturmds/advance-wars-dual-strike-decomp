#include "sdk/wl.h"

void func_arm7_027c94ec(int value);

int WifiParam_StoreRole(u16 phase) {
  if (phase > 3) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.phase = phase;
  WIFI_DRIVER_STATE->state.unk_24 = phase;
  WIFI_MAC_MODE_AND_WEP = (WIFI_MAC_MODE_AND_WEP & 0xfff8) | phase;
  func_arm7_027c94ec(WIFI_DRIVER_STATE->state.unk_26);
  WIFI_DRIVER_STATE->state.unk_14 |= 8;
  return 0;
}
