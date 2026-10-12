#include "sdk/wmsp.h"

WmspSlot* Wmsp_TakeFlaggedSlot(void);
void WMSP_QueueVAlarmMp(void);

void WMSP_VAlarmLine240(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  WMStatus* status;
  u64 now;
  WmspSlot* slot;
  BOOL queued;
  RadioNotice* indication;

  if (buf->status->mpFlag != 1) {
    return;
  }
  Wmsp_SetVAlarm(buf, 200);
  /* unk_7b0: the MP lifetime in ticks, 0 for none (WMSP_SetLifeTime) */
  if (buf->status->unk_7b0 != 0) {
    now = Tick_Read64_Arm7() | 1;
    status = buf->status;
    if (status->mpLastRecvTick[0] != 0 &&
        now - status->mpLastRecvTick[0] > status->unk_7b0) {
      status->mpLastRecvTick[0] = 0;
      slot = Wmsp_TakeFlaggedSlot();
      if (slot == NULL) {
        queued = FALSE;
      } else {
        slot->flags = WM_APIID_AUTO_DISCONNECT;
        slot->unk_04 = 0;
        slot->unk_08 = 0x8001;
        queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue,
                                        slot, OS_MESSAGE_NOBLOCK);
      }
      if (!queued) {
        indication = Wmsp_GetCallbackBuffer();
        indication->apiid = WM_APIID_INDICATION;
        indication->errcode = WM_ERRCODE_FIFO_ERROR;
        indication->state = WIRELESS_NOTICE_QUEUE_REFUSED;
        indication->reason = WM_APIID_AUTO_DISCONNECT;
        Wmsp_SendReply(indication);
      }
      return;
    }
  }
  WMSP_QueueVAlarmMp();
}
