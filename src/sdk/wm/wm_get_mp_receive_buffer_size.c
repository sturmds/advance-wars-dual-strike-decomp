#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Bytes for the MP receive buffer of a parent or child, 32-byte aligned and
 * doubled. 0 when the state is wrong or MP is running. */
int WM_GetMPReceiveBufferSize(void) {
  WMArm9Buf* buf = func_02116e10();
  BOOL isParent;
  u32 maxSize;

  if (WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD) != 0) {
    return 0;
  }
  DC_InvalidateRange(&buf->status->mpFlag, 4);
  if (buf->status->mpFlag == 1) {
    return 0;
  }
  DC_InvalidateRange(&buf->status->aid, 2);
  isParent = buf->status->aid == 0;
  DC_InvalidateRange(&buf->status->unk_190, 2);
  maxSize = buf->status->unk_190;
  if (isParent != TRUE) {
    return ((maxSize + 81) & ~31) << 1;
  }
  DC_InvalidateRange(&buf->status->unk_f4, 2);
  return (((maxSize + 12) * buf->status->unk_f4 + 41) & ~31) << 1;
}
