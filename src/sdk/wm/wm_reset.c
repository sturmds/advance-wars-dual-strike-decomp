#include "types.h"
#include "sdk/wm.h"

/* Returns the wireless hardware to the idle state. */
int WM_Reset(WMCallbackFunc callback) {
  int result = WMi_CheckIdle();

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_RESET, callback);
  result = WMi_SendCommand(WM_APIID_RESET, 0);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
