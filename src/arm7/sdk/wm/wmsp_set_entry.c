#include "sdk/wmsp.h"

void Wmsp_BuildGameInfo(WMGameInfo* info, const WMParentParam* param);
u16* func_arm7_027c1f70(WlCommand* cmd, int size, const void* data);

/* Sets whether the parent takes new children, then sends the beacon's game
   info again with the new flag. */
void WMSP_SetEntry(WmspRequest* request) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  u32 cmd[0x80];
  WMGameInfo gameInfo;
  u16* result;
  WmspReply* reply;

  Wmsp_ParentParam(status)->entryFlag = request->arg;
  Wmsp_BuildGameInfo(&gameInfo, Wmsp_ParentParam(status));
  /* Needed to match: the size is narrowed to 16 bits before the call. */
  result = func_arm7_027c1f70(
      (WlCommand*)cmd, (u16)(Wmsp_ParentParam(status)->userGameInfoLength + 16),
      &gameInfo);
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_SET_ENTRY;
  if (result[2] == 0) {
    reply->errcode = WM_ERRCODE_SUCCESS;
  } else {
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = 0x245; /* the WL command 0x027c1f70 sends */
    reply->wlResult = result[2];
  }
  Wmsp_SendReply(reply);
}
