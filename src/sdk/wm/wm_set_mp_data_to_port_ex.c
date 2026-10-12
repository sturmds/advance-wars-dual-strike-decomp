#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Queues data to send to the children in destBitmap (or to the parent) on a
 * port during the running MP exchange. callback runs with arg once it is
 * sent. Returns WM_ERRCODE_OPERATING when the command is queued. */
int WM_SetMPDataToPortEx(WMCallbackFunc callback, void* arg,
                         const void* sendData, u16 sendDataSize, u16 destBitmap,
                         u16 port, u16 prio) {
  int result;
  BOOL isParent;
  u16 maxSendDataSize;
  u16 childBitmap = 1;
  WMStatus* status = func_02116e10()->status;

  result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
  if (result != WM_ERRCODE_SUCCESS) {
    return result;
  }
  DC_InvalidateRange(&status->unk_c2, 2);
  if (status->unk_c2 != 1) {
    return WM_ERRCODE_ILLEGAL_STATE;
  }
  DC_InvalidateRange(&status->unk_18e, 2);
  maxSendDataSize = status->unk_18e;
  DC_InvalidateRange(&status->aid, 2);
  isParent = (status->aid == 0) ? TRUE : FALSE;
  if (isParent == TRUE) {
    DC_InvalidateRange(&status->childBitmap, 2);
    childBitmap = status->childBitmap;
    DC_InvalidateRange(&status->mpReadyBitmap, 2);
  }
  if (sendData == NULL) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  if (childBitmap == 0) {
    return WM_ERRCODE_NO_CHILD;
  }
  DC_InvalidateRange(&status->unk_50, 2);
  if (sendData == status->unk_50) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  /* the packet header takes 4 bytes on a parent, 2 on a child */
  if (sendDataSize + (isParent ? 4 : 2) > maxSendDataSize) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  if (sendDataSize == 0) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  DC_StoreRange(sendData, sendDataSize);
  result = WMi_SendCommand(WM_APIID_SET_MP_DATA, 7, sendData, sendDataSize,
                           destBitmap, port, prio, callback, arg);
  if (result == WM_ERRCODE_SUCCESS) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
