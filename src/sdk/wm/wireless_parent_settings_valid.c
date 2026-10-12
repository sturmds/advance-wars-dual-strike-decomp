#include "types.h"
#include "sdk/wm.h"

/* Returns TRUE when the parent settings are in range. */
BOOL Wireless_ParentSettingsValid(WMParentParam* param) {
  if (param->userGameInfoLength > 112) {
    return FALSE;
  }
  if (param->beaconPeriod < 10 || param->beaconPeriod > 1000) {
    return FALSE;
  }
  if (param->channel < 1 || param->channel > 14) {
    return FALSE;
  }
  return TRUE;
}
