#include "sdk/wmsp.h"

void WMSP_ReplyEndScanFailed(u16 wlCmdID, u16 wlResult) {
  WmspReply* reply = Wmsp_GetCallbackBuffer();

  reply->apiid = WM_APIID_END_SCAN;
  reply->errcode = WM_ERRCODE_FAILED;
  reply->wlCmdID = wlCmdID;
  reply->wlResult = wlResult;
  Wmsp_SendReply(reply);
}
