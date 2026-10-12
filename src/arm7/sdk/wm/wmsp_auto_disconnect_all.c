#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

WmspSlot *Wmsp_TakeFlaggedSlot(void);

/*
 * WL indication 0x186: queues an auto-disconnect of every link (all children on
 * a parent, the parent on a child) unless unk_da is 1.
 */
void Wmsp_AutoDisconnectAll(WlIndicationHeader *ind)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WmspAutoDisconnectRequest *request;
    BOOL queued;

    if (buf->status->unk_da == 1) {
        return;
    }
    request = (WmspAutoDisconnectRequest *)Wmsp_TakeFlaggedSlot();
    if (request == NULL) {
        queued = FALSE;
    } else {
        request->apiid = WM_APIID_AUTO_DISCONNECT;
        request->reason = 0x8003;
        if (buf->status->state == WM_STATE_MP_PARENT || buf->status->state == WM_STATE_PARENT) {
            request->aidBitmap = 0x7ffe; /* every child */
        } else if (buf->status->state == WM_STATE_MP_CHILD || buf->status->state == WM_STATE_CHILD) {
            request->aidBitmap = 1; /* the parent */
        }
        queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue, request, OS_MESSAGE_NOBLOCK);
    }
    if (!queued) {
        RadioNotice *error = Wmsp_GetCallbackBuffer();

        error->apiid = WM_APIID_INDICATION;
        error->errcode = WM_ERRCODE_FIFO_ERROR;
        error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
        error->reason = WM_APIID_AUTO_DISCONNECT;
        Wmsp_SendReply(error);
    }
}
