#include "types.h"
#include "sdk/wm.h"

/* Sets the timeouts after which the ARM7 drops a connection that has gone
 * silent. tgid is the parent's temporary group id. */
int WM_SetLifeTime(WMCallbackFunc callback, u16 tgid, u16 camLifeTime,
                   u16 frameLifeTime, u16 mpLifeTime) {
  int result = WMi_CheckIdle();

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_SET_LIFETIME, callback);
  result = WMi_SendCommand(WM_APIID_SET_LIFETIME, 4, tgid, camLifeTime,
                           frameLifeTime, mpLifeTime);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
