#include "types.h"
#include "sdk/wm.h"

/* Turns the wireless hardware off. */
int WM_PowerOff(WMCallbackFunc callback) {
  int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_POWER_OFF, callback);
  result = WMi_SendCommand(WM_APIID_POWER_OFF, 0);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
