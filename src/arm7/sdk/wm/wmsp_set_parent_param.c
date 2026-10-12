#include "sdk/wmsp.h"

u16* func_arm7_027c20c4(WlCommand* cmd, u16 arg);
void WMSP_ReplySetParentParamFailed(u16 wlCmdID, u16 wlResult);

/* As in sdk/mi.h, which clashes with wmsp.h over Mem_CopyHalfwords_Arm7. */

/* Keeps the parent parameters in WMStatus. The channel has to be one the
   firmware allows; maxEntry goes to the WL with command 0x212. */
void WMSP_SetParentParam(WmspSetParentParamRequest* request) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  WlCommandBuf cmd;
  WMStatus* status;
  u16* result;
  WmspReply* reply;

  Mem_CopyBytes_Arm7(request->param, Wmsp_ParentParam(buf->status),
                     sizeof(WMParentParam));
  status = buf->status;
  /* needed to match: a cast here, not Wmsp_ParentParam, keeps the load order */
  if (((1 << ((WMParentParam*)status->pparam)->channel) &
       status->enableChannel) == 0) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_P_PARAM;
    reply->errcode = WM_ERRCODE_INVALID_PARAM;
    Wmsp_SendReply(reply);
    return;
  }
  result = func_arm7_027c20c4(&cmd.command, Wmsp_ParentParam(status)->maxEntry);
  if (result[2] != 0) {
    WMSP_ReplySetParentParamFailed(0x212, result[2]);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_SET_P_PARAM;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
