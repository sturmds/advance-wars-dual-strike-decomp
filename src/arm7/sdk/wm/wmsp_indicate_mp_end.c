#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

WmspSlot* Wmsp_TakeFlaggedSlot(void);
void Wmsp_WaitCallbackFree(void);
// The original passes three arguments; Wmsp_UpdateMinRssi reads two.
void Wmsp_UpdateMinRssi(WlMpEndIndication* ind, u16 bitmap, u16 unk_92);
int WMSP_UpdateSendQueue(BOOL timedOut, u16 polledAgain);

/* WIFI_HOSTMSG_POLL_FINISHED on a parent: the children's replies to an MP frame arrived.
   If a child's reply failed, queues request 42 and stops. Otherwise copies
   the replies into the next receive buffer, passes each child's data on,
   disconnects a child that sent nothing for longer than WMStatus.unk_7b0
   ticks, reports MPEND_IND to the ARM9, and queues request 40 for the next
   exchange while WMStatus.unk_36 says more are due. */
void Wmsp_IndicateMpEnd(WlMpEndIndication* ind) {
  // Needed to match: this declaration order sets the register allocation.
  WmspInternalRequest* request;
  u16 i;
  BOOL queued;
  WMStatus* status;
  WMMpRecvBuf* recvBuf;
  u64 last;
  WmspWork* work;
  WMStartMPCallback* reply;
  WmspAutoDisconnectRequest* disconnect;
  u32 size;
  RadioNotice* error;
  WMMpRecvData* entry;
  u64 now;
  BOOL resendMissing;
  BOOL more;
  int intr;
  u32 arg;

  status = func_arm7_027c05e0()->buf->status;
  resendMissing = FALSE;

  if (status->mpFlag == 0) {
    return;
  }
  if (ind->recv.errBitmap != 0 ||
      (status->unk_38 == 1 && ind->recv.bitmap != 0)) {
    request = (WmspInternalRequest*)Wmsp_TakeFlaggedSlot();
    if (request == NULL) {
      queued = FALSE;
    } else {
      request->apiid = 42;
      request->arg = status->unk_3c;
      queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue,
                                      request, OS_MESSAGE_NOBLOCK);
    }
    if (queued) {
      status->unk_38 = 1;
      return;
    }
    error = Wmsp_GetCallbackBuffer();
    error->apiid = WM_APIID_INDICATION;
    error->errcode = WM_ERRCODE_FIFO_ERROR;
    error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
    error->reason = 42;
    Wmsp_SendReply(error);
    return;
  }
  if (status->unk_38 != 0) {
    status->unk_38 = 0;
  }

  recvBuf = (WMMpRecvBuf*)status->mpRecvBuf[status->mpRecvBufSel];
  size = ind->recv.stride * ind->recv.count + 10;
  if (status->mpRecvBufSize < size) {
    size = status->mpRecvBufSize;
  }
  Mem_CopyBytes_Arm7(&ind->recv, recvBuf, size);
  Wmsp_UpdateMinRssi(ind, recvBuf->bitmap, status->unk_92);

  now = Tick_Read64_Arm7() | 1;
  entry = (WMMpRecvData*)recvBuf->firstEntry;
  for (i = 0; i < recvBuf->count;
       i++, entry = (WMMpRecvData*)((u8*)entry + recvBuf->stride)) {
    if (entry->aid >= 1 && entry->aid <= 15) {
      if (entry->length >= 2 && entry->length != 0xffff) {
        entry->length -= 2;
        status->mpReadyBitmap |= 1 << entry->aid;
        status->mpLastRecvTick[entry->aid] = now;
        if (entry->length != 0) {
          // Offset 8 (WMMpRecvData.timeStamp) holds the entry's WM header here.
          WMSP_ParsePortPacket(entry->aid, entry->timeStamp, (u16*)entry->cdata,
                               entry->length, recvBuf);
        }
      } else if (entry->length == 0) {
        last = status->mpLastRecvTick[entry->aid];
        if (status->unk_7b0 != 0 && last != 0 && now - last > status->unk_7b0) {
          disconnect = (WmspAutoDisconnectRequest*)Wmsp_TakeFlaggedSlot();
          status->mpLastRecvTick[entry->aid] = 0;
          if (disconnect == NULL) {
            queued = FALSE;
          } else {
            disconnect->apiid = WM_APIID_AUTO_DISCONNECT;
            disconnect->aidBitmap = 1 << entry->aid;
            disconnect->reason = 0x8001;
            queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue,
                                            disconnect, OS_MESSAGE_NOBLOCK);
          }
          if (!queued) {
            error = Wmsp_GetCallbackBuffer();
            error->apiid = WM_APIID_INDICATION;
            error->errcode = WM_ERRCODE_FIFO_ERROR;
            error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
            error->reason = WM_APIID_AUTO_DISCONNECT;
            Wmsp_SendReply(error);
          }
        }
      }
    }
  }

  WMSP_UpdateSendQueue(FALSE, recvBuf->bitmap);
  if (recvBuf->bitmap != 0 && status->unk_96 == 0 && status->unk_34 != 0) {
    resendMissing = TRUE;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_START_MP;
  reply->errcode = WM_ERRCODE_SUCCESS;
  reply->state = WM_STATECODE_MPEND_IND;
  reply->recvBuf = recvBuf;
  Wmsp_SendReply(reply);
  Wmsp_WaitCallbackFree();
  status->mpRecvBufSel ^= 1;

  intr = Cpu_MaskIrq_Arm7();
  if (!resendMissing) {
    status->unk_36--;
  }
  more = status->unk_36 != 0;
  Cpu_RestoreIrqBit_Arm7(intr);
  if (!more) {
    return;
  }

  if (resendMissing == TRUE) {
    arg = ind->recv.bitmap;
  } else {
    arg = 0xffff; /* every child */
  }
  request = (WmspInternalRequest*)Wmsp_TakeFlaggedSlot();
  work = func_arm7_027c05e0();
  if (request == NULL) {
    queued = FALSE;
  } else {
    request->apiid = 40;
    request->arg = arg;
    queued = MsgQueue_PushBack_Arm7(&work->requestQueue, request,
                                    OS_MESSAGE_NOBLOCK);
  }
  if (!queued && work->buf != NULL) {
    error = Wmsp_GetCallbackBuffer();
    error->apiid = WM_APIID_INDICATION;
    error->errcode = WM_ERRCODE_FIFO_ERROR;
    error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
    error->reason = 40;
    Wmsp_SendReply(error);
  }
}
