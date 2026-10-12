#include "sdk/wl.h"

void func_arm7_027c87cc(int a0);
void WifiRf_LoadStartValues(void);

void func_arm7_027c93a4(void) {
  u16 value;

  WIFI_MAC_REGISTER(0x036) = 0;
  func_arm7_027c87cc(8);
  WIFI_MAC_REGISTER(0x168) = 0;
  switch (WIFI_DRIVER_STATE->rf.type) {
    case 2:
      value = WifiBaseband_Get(1);
      WifiBaseband_Put(1, value & 0x7f);
      WifiBaseband_Put(1, value);
      func_arm7_027c87cc(40);
      WifiRf_LoadStartValues();
      break;
    case 3:
      WifiRf_LoadStartValues();
      break;
  }
}
