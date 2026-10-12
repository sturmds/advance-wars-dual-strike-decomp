#include "sdk/pm.h"
#include "sdk/wmsp.h"

u16* func_arm7_027c1f14(WlCommand* cmd);
void WMSP_ReplyEndFailed(u16 wlCmdID, u16 wlResult);

/* Only allowed in WM_STATE_IDLE: stops the wireless firmware (WL command
   0x301), changes the power LED pattern back and leaves WM in WM_STATE_READY. */
void WMSP_End(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  WlCommandBuf cmd;
  u16* result;
  WmspReply* reply;

  if (buf->status->state != WM_STATE_IDLE) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_END;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    Wmsp_SendReply(reply);
    return;
  }
  result = func_arm7_027c1f14(&cmd.command);
  if (result[2] != 0) {
    WMSP_ReplyEndFailed(0x301, result[2]);
    return;
  }
  buf->status->state = WM_STATE_STOP;
  PowerLed_SelectPattern(1); /* WMSP_Initialize set pattern 15 */
  buf->status->state = WM_STATE_READY;
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_END;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
