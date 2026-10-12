#include "sdk/wmsp.h"

u16* func_arm7_027c224c(WlCommand* cmd, u16 arg);
void WMSP_ReplyEndDCFFailed(u16 wlCmdID, u16 wlResult);

void WMSP_EndDCF(void) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  u32 wlBuf[WMSP_WL_BUF_WORDS];
  WmspReply* reply;
  u16* wlReply;
  int irqState = Cpu_MaskIrq_Arm7();

  if (status->state != WM_STATE_DCF_CHILD) {
    Cpu_RestoreIrqBit_Arm7(irqState);
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_END_DCF;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    Wmsp_SendReply(reply);
    return;
  }
  status->unk_10 = 0;
  status->state = WM_STATE_CHILD;
  Cpu_RestoreIrqBit_Arm7(irqState);

  wlReply = func_arm7_027c224c((WlCommand*)wlBuf, 7);
  if (wlReply[2] != 0) {
    WMSP_ReplyEndDCFFailed(0x104, wlReply[2]);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_END_DCF;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
