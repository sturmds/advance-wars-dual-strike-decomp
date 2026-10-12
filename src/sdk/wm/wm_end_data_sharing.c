#include "types.h"
#include "sdk/wm.h"

void DataShare_OnParentPortRecv(void* arg);
void DataShare_OnChildPortRecv(void* arg);

/* Stops data sharing: frees its port and resets dsInfo. */
int WM_EndDataSharing(WMDataSharingInfo* dsInfo) {
  WMArm9Buf* buf = func_02116e10();
  int result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);

  if (result != 0) {
    return result;
  }
  if (dsInfo == NULL) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  /* not started, or the port was given to something else since */
  if (dsInfo->aidBitmap == 0 ||
      (buf->portCallback[dsInfo->port] != DataShare_OnParentPortRecv &&
       buf->portCallback[dsInfo->port] != DataShare_OnChildPortRecv)) {
    return WM_ERRCODE_ILLEGAL_STATE;
  }
  WM_SetPortCallback(dsInfo->port, NULL, NULL);
  dsInfo->aidBitmap = 0;
  dsInfo->state = SHARE_PHASE_IDLE;
  return WM_ERRCODE_SUCCESS;
}
