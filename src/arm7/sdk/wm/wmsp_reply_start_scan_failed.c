#include "sdk/wmsp.h"

void WMSP_ReplyStartScanFailed(u16 wlCmdID, u16 wlResult, BOOL extended) {
  WMStartScanCallback* reply = Wmsp_GetCallbackBuffer();

  if (extended) {
    reply->apiid = WM_APIID_START_SCAN_EX;
  } else {
    reply->apiid = WM_APIID_START_SCAN;
  }
  reply->errcode = WM_ERRCODE_FAILED;
  reply->state = WM_STATECODE_PARENT_NOT_FOUND;
  reply->wlCmdID = wlCmdID;
  reply->wlResult = wlResult;
  Wmsp_SendReply(reply);
}
