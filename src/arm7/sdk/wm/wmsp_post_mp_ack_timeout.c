#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

void func_arm7_027c1a98(WlIndication *ind);

/*
 * Alarm handler for a missing MP ack: queues a made-up WIFI_HOSTMSG_POLL_ACKED with
 * unk_0e 0, so Wmsp_IndicateMpAck reports the timeout on the WMSP thread.
 */
void Wmsp_PostMpAckTimeout(void *arg)
{
    WmspWork *work = func_arm7_027c05e0();
    WlIndication *ind = Heap_TakeFirstFit_Arm7(work->heapArena, work->heap, sizeof(WlMpAckIndication));

    ind->code = WIFI_HOSTMSG_POLL_ACKED;
    ind->unk_0e = 0;
    if (!MsgQueue_PushBack_Arm7(&work->indicationQueue, ind, OS_MESSAGE_NOBLOCK)) {
        func_arm7_027c1a98(ind);
        if (work->buf != NULL) {
            RadioNotice *error = Wmsp_GetCallbackBuffer();

            error->apiid = WM_APIID_INDICATION;
            error->errcode = WM_ERRCODE_FIFO_ERROR;
            error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
            error->reason = WM_APIID_INDICATION;
            Wmsp_SendReply(error);
        }
    }
}
