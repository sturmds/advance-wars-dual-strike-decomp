#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

u16* func_arm7_027c1efc(WlCommand* cmd);
u16* func_arm7_027c2648(WlCommand* cmd, const void* src, int arg);
u16* func_arm7_027c28f4(WlCommand* cmd, int arg);
void WMSP_ReplyEndParentFailed(u16 wlCmdID, u16 wlResult);

/* As in sdk/mi.h, which clashes with wmsp.h over Mem_CopyHalfwords_Arm7. */

/* Only allowed in WM_STATE_PARENT: disconnects every child with WL command 5
   (argument 3), stops the parent and goes back to WM_STATE_IDLE through
   class 1. */
void WMSP_EndParent(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  u8 macAddress[8]; /* needed to match: 6 used, 8 on the stack */
  WlCommandBuf cmd;
  WmspReply* reply;
  u16* result;
  int aid;
  u32 mask;
  int irq;

  if (buf->status->state != WM_STATE_PARENT) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_END_PARENT;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    Wmsp_SendReply(reply);
    return;
  }
  for (aid = 1; aid < 16; aid++) {
    WMStatus* status = buf->status;

    mask = 1 << aid;
    if (status->childBitmap & mask) {
      Mem_CopyBytes_Arm7(status->childMacAddress[aid - 1], macAddress, 6);
      func_arm7_027c2648(&cmd.command, macAddress, 3);
      irq = Cpu_MaskIrq_Arm7();
      buf->status->childBitmap &= ~mask;
      buf->status->mpReadyBitmap &= ~mask;
      buf->status->mpLastRecvTick[aid] = 0;
      Cpu_RestoreIrqBit_Arm7(irq);
    }
  }
  result = func_arm7_027c28f4(&cmd.command, 1);
  if (result[2] != 0) {
    WMSP_ReplyEndParentFailed(0, result[2]);
    return;
  }
  buf->status->unk_be = 0;
  buf->status->state = WM_STATE_CLASS1;
  result = func_arm7_027c1efc(&cmd.command);
  if (result[2] != 0) {
    WMSP_ReplyEndParentFailed(0x302, result[2]);
    return;
  }
  buf->status->state = WM_STATE_IDLE;
  buf->status->unk_18e = 0;
  buf->status->unk_190 = 0;
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_END_PARENT;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
