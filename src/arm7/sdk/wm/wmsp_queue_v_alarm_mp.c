#include "sdk/wmsp.h"

WmspSlot* Wmsp_TakeFlaggedSlot(void);

void WMSP_QueueVAlarmMp(void) {
  WmspWork* work = func_arm7_027c05e0();
  WMStatus* status = work->buf->status;
  int irqState = Cpu_MaskIrq_Arm7();
  WmspSlot* slot;
  BOOL queued;
  RadioNotice* indication;

  /* unk_ca is 1 while a VALARM_MP request waits in the queue */
  if (status->unk_ca == 1) {
    Cpu_RestoreIrqBit_Arm7(irqState);
    return;
  }
  status->unk_ca = 1;
  Cpu_RestoreIrqBit_Arm7(irqState);

  slot = Wmsp_TakeFlaggedSlot();
  if (slot == NULL) {
    queued = FALSE;
  } else {
    slot->flags = WM_APIID_VALARM_MP;
    queued =
        MsgQueue_PushBack_Arm7(&work->requestQueue, slot, OS_MESSAGE_NOBLOCK);
  }
  if (!queued) {
    status->unk_ca = 0;
    if (work->buf != NULL) {
      indication = Wmsp_GetCallbackBuffer();
      indication->apiid = WM_APIID_INDICATION;
      indication->errcode = WM_ERRCODE_FIFO_ERROR;
      indication->state = WIRELESS_NOTICE_QUEUE_REFUSED;
      indication->reason = WM_APIID_VALARM_MP;
      Wmsp_SendReply(indication);
    }
  }
}
