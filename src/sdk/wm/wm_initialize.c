#include "types.h"
#include "sdk/wm.h"

/* Sets up WM on the ARM9, then has the ARM7 side start up with the work
 * area's pieces. workArea needs WM_SYSTEM_BUF_SIZE bytes, 32-byte aligned. */
int WM_Initialize(void* workArea, WMCallbackFunc callback, u16 dmaNo) {
  WMArm9Buf* buf;
  int result = WM_Init(workArea, dmaNo);

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_INITIALIZE, callback);
  buf = func_02116e10();
  result = WMi_SendCommand(WM_APIID_INITIALIZE, 3, buf->arm7Buf, buf->status,
                           buf->replyBuf);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
