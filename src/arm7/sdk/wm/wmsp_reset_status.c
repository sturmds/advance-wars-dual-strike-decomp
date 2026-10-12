#include "sdk/wmsp.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"

void Wmsp_SetNormalThreadPriorities(void);
void func_arm7_027c632c(u32 aidBitmap);
void func_arm7_027c5de8(void);

/* Stops MP if it runs, clears the connection fields of WMStatus, frees every
   slot and resets the port sequence numbers. */
void WMSP_ResetStatus(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  BOOL mpWasOn = FALSE;
  int intr;
  int i;
  volatile u16 one; /* needed to match: the 1 goes through the stack */

  intr = Cpu_MaskIrq_Arm7();
  if (buf->status->mpFlag == 1) {
    buf->status->mpFlag = 0;
    mpWasOn = TRUE;
    LineAlarm_Withdraw_Arm7(&buf->vAlarm);
    Wmsp_SetNormalThreadPriorities();
  }
  buf->status->childBitmap = 0;
  buf->status->mpReadyBitmap = 0;
  buf->status->unk_14 = 0;
  buf->status->unk_10 = 0;
  buf->status->unk_1c = 0;
  buf->status->unk_92 = 0;
  buf->status->unk_94 = 0;
  buf->status->unk_96 = 0;
  buf->status->unk_18e = 0;
  buf->status->unk_190 = 0;
  buf->status->unk_ca = 0;
  buf->status->unk_be = 0;
  buf->status->unk_c2 = 0;
  Cpu_RestoreIrqBit_Arm7(intr);
  if (mpWasOn) {
    func_arm7_027c632c(0xffff);
  }
  for (i = 0; i < WMSP_SLOT_COUNT; i++) {
    buf->slots[i].flags = WMSP_SLOT_FLAG;
  }
  one = 1;
  Mem_FillHalfwords_Arm7(one, buf->status->portSeqNo,
                         sizeof(buf->status->portSeqNo));
  Alarm_Clear_Arm7(&buf->status->mpAckAlarm);
  Mutex_Clear_Arm7(&buf->status->sendQueueMutex);
  func_arm7_027c5de8();
}
