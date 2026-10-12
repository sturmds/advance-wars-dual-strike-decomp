#include "sdk/wmsp.h"

/* Needed to match: the early return gets its own epilogue, as -O4,p builds it. */
#pragma optimize_for_size off

int WMSP_BuildMpSendData(int a0, int a1, int a2, int a3, u16* flags,
                         const void* buf, u32* a6, u32* size);
u16 func_arm7_027c6020(u32 parentSize, u32 childSize, u32 childBitmap);
u16* func_arm7_027c22a8(WlCommand* cmd, u16 arg0, u16 arg1, u16 arg2, u16 arg3,
                        u16 arg4, u16 arg5, u16 arg6, u16 arg7, u32 arg8);

/*
 * A parent's MP send, from request slot 40 and WMSP_VAlarmMp: WMSP_BuildMpSendData
 * fills the send buffer from the port queues (it returns 1 when there is nothing
 * to send), then WL command 0x102 sends it to the polled children. When unk_9a
 * is set, an empty frame goes to all children instead. The command's sizes and
 * flags are kept in unk_3c..unk_42 for WMSP_ResendMp.
 */
void WMSP_SendMp(u16 arg) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  int irqState;
  u32 pollBitmap;
  const void* buf;
  u16 readyBitmap;
  u16 childBitmap;
  u16 maxSize;
  u16 flags;
  u32 polled;
  u32 parentSize;
  u32 childSize;
  u16 time;
  u32 cmd[0x81];

  pollBitmap = arg;
  irqState = Cpu_MaskIrq_Arm7();
  childBitmap = status->childBitmap;
  readyBitmap = status->mpReadyBitmap;
  Cpu_RestoreIrqBit_Arm7(irqState);
  buf = status->unk_50;
  maxSize = status->unk_18e;
  flags = 0;
  if (status->unk_88 == 1) {
    pollBitmap = 0xffff;
    status->unk_88 = 0;
  }
  if (status->unk_9a == 0) {
    if (WMSP_BuildMpSendData(pollBitmap, childBitmap, readyBitmap, maxSize,
                             &flags, buf, &polled, &parentSize) == 1) {
      return;
    }
    childSize = status->unk_190 + 2;
    if (status->unk_92 == 1) {
      pollBitmap = polled;
    }
    pollBitmap &= childBitmap;
    if (status->unk_36 == 1) {
      time = func_arm7_027c6020(parentSize, childSize, pollBitmap);
      flags |= 0x8000;
    } else {
      time = 0;
    }
  } else {
    childSize = 0;
    status->unk_9a = 0;
    pollBitmap = childBitmap;
    time = 0;
    parentSize = 0;
    flags &= ~0x8000;
    status->unk_36++;
  }
  func_arm7_027c22a8((WlCommand*)cmd, 0, 0, childSize, pollBitmap, time,
                     W_US_COUNT0, parentSize, flags, (u32)buf);
  status->unk_3c = pollBitmap;
  status->unk_3e = flags;
  status->unk_40 = childSize;
  status->unk_42 = parentSize;
}
