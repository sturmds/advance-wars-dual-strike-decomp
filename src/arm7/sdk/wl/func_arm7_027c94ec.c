#include "sdk/wl.h"

// The definition has a third, unused parameter; callers pass only two.
int WifiParam_StorePostBeaconTime(u16 value, int writeReg);

int func_arm7_027c94ec(u16 enable) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlHead* head = WIFI_SESSION_HEAD_OF(work);

  work->state.unk_26 = enable;
  if (enable != 0 && head->phase != 1) {
    WIFI_MAC_MODE_AND_WEP |= 0x40;
  } else {
    WIFI_MAC_MODE_AND_WEP &= ~0x40;
    WifiParam_StorePostBeaconTime(head->unk_20, 0);
  }
  return 0;
}
