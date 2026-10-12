#include "sdk/wmsp.h"

void WMSP_ReplyDisconnectFailed(u16 wlCmdID, u16 wlResult, u16 tryBitmap,
                                u16 disconnectedBitmap) {
  RadioDropReply* reply = Wmsp_GetCallbackBuffer();

  reply->apiid = WM_APIID_DISCONNECT;
  reply->errcode = WM_ERRCODE_FAILED;
  reply->wlCmdID = wlCmdID;
  reply->wlResult = wlResult;
  reply->tryBitmap = tryBitmap;
  reply->disconnectedBitmap = disconnectedBitmap;
  Wmsp_SendReply(reply);
}
