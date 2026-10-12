#include "types.h"
#include "sdk/wm.h"

/* Shuts the wireless manager down. */
int WM_End(WMCallbackFunc callback) {
  int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_END, callback);
  result = WMi_SendCommand(WM_APIID_END, 0);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
