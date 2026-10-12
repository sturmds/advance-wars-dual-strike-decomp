#include "sdk/wmsp.h"

#define WIFI_REQ_ZERO_STATS 0x305 /* named after the request it serves */

u16* func_arm7_027c1eb8(WlCommand* cmd);
void WMSP_ReplyInitWCounterFailed(u16 wlCmdID, u16 wlResult);

void WMSP_InitWCounter(void) {
  WlCommandBuf buf;
  u16* wlReply;
  WmspReply* reply;

  func_arm7_027c05e0(); /* the original fetches the work area and never uses it */
  wlReply = func_arm7_027c1eb8(&buf.command);
  if (wlReply[2] != 0) {
    WMSP_ReplyInitWCounterFailed(WIFI_REQ_ZERO_STATS, wlReply[2]);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_INIT_W_COUNTER;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
