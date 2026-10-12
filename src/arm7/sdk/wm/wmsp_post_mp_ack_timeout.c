#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

void Wmsp_FreeIndication(WlIndicationHeader* ind);

/*
 * Alarm handler for a missing MP ack: queues a made-up WIFI_HOSTMSG_POLL_ACKED with
 * length 0, so Wmsp_IndicateMpAck reports the timeout on the WMSP thread.
 */
void Wmsp_PostMpAckTimeout(void* arg) {
  WmspWork* work = func_arm7_027c05e0();
  WlIndicationHeader* ind = Heap_TakeFirstFit_Arm7(work->heapArena, work->heap,
                                                   sizeof(WlMpAckIndication));

  ind->command = WIFI_HOSTMSG_POLL_ACKED;
  ind->length = 0;
  if (!MsgQueue_PushBack_Arm7(&work->indicationQueue, ind,
                              OS_MESSAGE_NOBLOCK)) {
    Wmsp_FreeIndication(ind);
    if (work->buf != NULL) {
      RadioNotice* error = Wmsp_GetCallbackBuffer();

      error->apiid = WM_APIID_INDICATION;
      error->errcode = WM_ERRCODE_FIFO_ERROR;
      error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
      error->reason = WM_APIID_INDICATION;
      Wmsp_SendReply(error);
    }
  }
}
