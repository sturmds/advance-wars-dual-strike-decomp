#include "sdk/wmsp.h"

u16* func_arm7_027c2064(WlCommand* cmd, u16 arg);
void WMSP_ReplySetBeaconIndFailed(u16 wlCmdID, u16 wlResult);

void WMSP_SetBeaconInd(WmspRequest* request) {
  u32 wlBuf[WMSP_WL_BUF_WORDS];
  u16* wlReply;
  WmspReply* reply;

  func_arm7_027c05e0();
  wlReply = func_arm7_027c2064((WlCommand*)wlBuf, request->arg);
  if (wlReply[2] != 0) {
    WMSP_ReplySetBeaconIndFailed(0x215, wlReply[2]);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_SET_BEACON_IND;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
