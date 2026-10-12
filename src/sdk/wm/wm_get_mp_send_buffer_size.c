#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Bytes for the MP send buffer, rounded up to 32. 0 when the state is wrong or
 * MP is running. */
int WM_GetMPSendBufferSize(void) {
  WMArm9Buf* buf = func_02116e10();

  if (WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD) != 0) {
    return 0;
  }
  DC_InvalidateRange(&buf->status->mpFlag, 4);
  if (buf->status->mpFlag == 1) {
    return 0;
  }
  DC_InvalidateRange(&buf->status->unk_18e, 4);
  return (buf->status->unk_18e + 31) & ~31;
}
