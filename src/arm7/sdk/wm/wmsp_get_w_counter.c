#include "sdk/wmsp.h"

#define WIFI_REQ_READ_STATS 0x307 /* named after the request it serves */

u16* func_arm7_027c1e88(WlCommand* cmd);
void WMSP_ReplyGetWCounterFailed(u16 wlCmdID, u16 wlResult);

void WMSP_GetWCounter(void) {
  WlCommandBuf buf;
  u16* wlReply;
  WmspWCounterReply* reply;

  func_arm7_027c05e0(); /* the original fetches the work area and never uses it */
  wlReply = func_arm7_027c1e88(&buf.command);
  if (wlReply[2] != 0) {
    WMSP_ReplyGetWCounterFailed(WIFI_REQ_READ_STATS, wlReply[2]);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->header.apiid = WM_APIID_GET_W_COUNTER;
  reply->header.errcode = WM_ERRCODE_SUCCESS;
  Mem_CopyHalfwords_Arm7(&wlReply[4], reply->counters, sizeof(reply->counters));
  Wmsp_SendReply(reply);
}
