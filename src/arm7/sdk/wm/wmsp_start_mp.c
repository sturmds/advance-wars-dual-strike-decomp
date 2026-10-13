#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

void WMSP_InitSendQueue(void);
void Wmsp_SetAltThreadPriorities(void);
void WMSP_StartMpVAlarm(void);

/* Starts MP from WM_STATE_PARENT or WM_STATE_CHILD: stops a running MP
   first, resets the MP state in WMStatus from the request (recvBuf holds two
   buffers of recvBufSize bytes), starts the MP alarm and moves to the MP
   state. */
void WMSP_StartMP(WmspStartMPRequest* request) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  WMStatus* status = buf->status;
  void* recvBuf = request->recvBuf;
  u16 recvBufSize = request->recvBufSize;
  BOOL wasRunning;
  WMStartMPCallback* reply;
  int irq;
  OSTick tick;
  int i;

  if (status->unk_e2 == 2 &&
      (status->allowedChannel & ((1 << buf->unk_46) >> 1)) == 0) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_MP;
    reply->errcode = WM_ERRCODE_INVALID_PARAM;
    reply->state = WM_STATECODE_MP_START;
    Wmsp_SendReply(reply);
    return;
  }
  wasRunning = FALSE;
  if (status->mpFlag != 0) {
    status->mpFlag = 0;
    wasRunning = TRUE;
  }
  if (wasRunning) {
    SendQueue_DropChildren(0xffff);
  }
  WMSP_InitSendQueue();
  irq = Cpu_MaskIrq_Arm7();
  if (status->state == WM_STATE_PARENT || status->state == WM_STATE_CHILD) {
    status->mpWaitAckFlag = 0;
    status->unk_30 = 0;
    status->unk_32 = 1;
    status->unk_88 = 0;
    status->unk_9a = 0;
    status->unk_9c = 60;
    status->sendQueueInUse = 0;
    status->unk_8a = 0;
    status->unk_8c = 0;
    status->unk_8e = 0;
    status->unk_90 = 0;
    status->unk_38 = 0;
    status->mpRecvBuf[0] = recvBuf;
    status->mpRecvBufSize = recvBufSize;
    status->mpRecvBuf[1] = (RadioChildFrameBuffer*)((u8*)recvBuf + recvBufSize);
    status->mpRecvBufSel = 0;
    status->unk_50 = request->sendBuf;
    status->unk_54 = request->sendBufSize;
    status->unk_34 = request->mpFreq;
    if (status->unk_34 == 0) {
      status->unk_34 = 16;
    }
    status->unk_98 = request->defaultRetryCount;
    status->unk_92 = request->minPollBmpMode != 0;
    status->unk_94 = request->singlePacketMode != 0;
    status->unk_96 = request->fixFreqMode != 0;
    status->unk_36 = 0;
    status->unk_3c = 0;
    status->unk_3e = 0;
    status->unk_ba = 0xffff;
    status->unk_bc = 4;
    status->unk_da = request->ignoreFatalError != 0;
    tick = Tick_Read64_Arm7() | 1;
    for (i = 0; i < 16; i++) {
      status->mpLastRecvTick[i] = tick;
    }
    Wmsp_SetAltThreadPriorities();
    status->unk_ca = 0;
    WMSP_StartMpVAlarm();
    if (status->state == WM_STATE_CHILD) {
      status->state = WM_STATE_MP_CHILD;
    } else if (status->state == WM_STATE_PARENT) {
      status->state = WM_STATE_MP_PARENT;
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_MP;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WM_STATECODE_MP_START;
    Wmsp_SendReply(reply);
    status->mpFlag = 1;
    Cpu_RestoreIrqBit_Arm7(irq);
  } else {
    Cpu_RestoreIrqBit_Arm7(irq);
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_MP;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    reply->state = WM_STATECODE_MP_START;
    Wmsp_SendReply(reply);
  }
}
