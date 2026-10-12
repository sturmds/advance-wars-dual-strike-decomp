#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

void DataShare_OnParentPortRecv(void* arg);
void DataShare_OnChildPortRecv(void* arg);

/* Runs when a data-sharing set has been sent: the parent records the set's
 * MP sequence number. When the send queue was full, the set is sent again
 * later. */
void DataShare_OnSetSent(void* arg) {
  WMPortSendCallback* cb = arg;
  WMArm9Buf* buf = func_02116e10();
  WMStatus* status = buf->status;
  WMCallbackFunc recvCallback = buf->portCallback[cb->port];
  WMDataSharingInfo* dsInfo = buf->portCallbackArg[cb->port];
  u16 aid;

  /* ignore the port once data sharing has stopped using it */
  if (recvCallback != DataShare_OnParentPortRecv &&
      recvCallback != DataShare_OnChildPortRecv) {
    return;
  }
  if (dsInfo == NULL || dsInfo != cb->arg) {
    return;
  }
  DC_InvalidateRange(&status->aid, 2);
  aid = status->aid;
  if (cb->errcode == WM_ERRCODE_SUCCESS) {
    if (aid == 0) {
      dsInfo->seqNum[dsInfo->sendIndex] = cb->seqNo >> 1;
      dsInfo->sendIndex = (dsInfo->sendIndex + 1) & (SHARE_RING_SLOTS - 1);
    }
  } else if (cb->errcode == WM_ERRCODE_SEND_QUEUE_FULL) {
    if (aid != 0) {
      /* step back one set */
      dsInfo->sendIndex =
          (dsInfo->sendIndex + SHARE_RING_SLOTS - 1) & (SHARE_RING_SLOTS - 1);
    }
    dsInfo->state = SHARE_PHASE_RESEND;
  } else {
    dsInfo->state = SHARE_PHASE_FAILED;
  }
}
