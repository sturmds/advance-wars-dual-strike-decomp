#include "sdk/wmsp.h"

void WMSP_ReplyStartParentFailed(u16 wlCmdID, u16 wlResult) {
  WMStartParentCallback* reply = Wmsp_GetCallbackBuffer();

  reply->apiid = WM_APIID_START_PARENT;
  reply->errcode = WM_ERRCODE_FAILED;
  reply->state = WM_STATECODE_PARENT_START;
  reply->wlCmdID = wlCmdID;
  reply->wlResult = wlResult;
  Wmsp_SendReply(reply);
}
