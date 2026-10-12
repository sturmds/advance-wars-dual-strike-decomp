#include "sdk/wmsp.h"

/* Request slot 35: sets WMStatus.unk_34 (0 means 16) and lowers unk_36 to it
   if needed, then replies with success. */
void func_arm7_027c7984(WmspRequest* request) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  u16 limit = request->arg;
  int intr;
  WmspReply* reply;

  if (limit == 0) {
    limit = 16;
  }
  intr = Cpu_MaskIrq_Arm7();
  status->unk_34 = limit;
  if (status->unk_36 > limit) {
    status->unk_36 = limit;
  }
  Cpu_RestoreIrqBit_Arm7(intr);
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = 35;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
