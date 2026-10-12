#include "sdk/wmsp.h"

/* Queues the data on a port; returns a WIRELESS_RESULT_*. */
int func_arm7_027c6824(u16 childBitmap, u16 prio, u16 port, u32 destBitmap, const u16 *data, u16 size,
                       void (*callback)(void *arg), void *arg);

void WMSP_SetMPData(WmspSetMPDataRequest *request)
{
    /* Needed to match: declared apart from the assignments, in this order. */
    WMStatus *status;
    const u16 *data;
    u16 size;
    u32 destBitmap;
    u16 port;
    u16 prio;
    void (*callback)(void *arg);
    void *arg;
    int result;
    u16 childBitmap;
    WMPortSendCallback *reply;

    status = func_arm7_027c05e0()->buf->status;
    childBitmap = status->childBitmap;
    data = request->data;
    size = request->size;
    destBitmap = request->destBitmap;
    port = request->port;
    prio = request->prio;
    callback = request->callback;
    arg = request->arg;

    /* A child only sends to the parent. */
    if (status->aid != 0) {
        destBitmap = 1;
    }
    if (status->mpFlag == 0) {
        result = WM_ERRCODE_ILLEGAL_STATE;
    } else if ((destBitmap & childBitmap) == 0) {
        result = WM_ERRCODE_SUCCESS;
    } else {
        result = func_arm7_027c6824(childBitmap, prio, port, destBitmap, data, size, callback, arg);
    }
    if (result != WM_ERRCODE_OPERATING) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_PORT_SEND;
        reply->errcode = result;
        reply->state = WM_STATECODE_PORT_SEND;
        reply->port = port;
        reply->destBitmap = destBitmap;
        if (result == WM_ERRCODE_SEND_QUEUE_FULL) {
            reply->restBitmap = destBitmap & childBitmap;
        } else {
            reply->restBitmap = 0;
        }
        reply->sentBitmap = 0;
        reply->size = size;
        reply->data = data;
        reply->callback = callback;
        reply->arg = arg;
        reply->seqNo = 0xffff;
        Wmsp_SendReply(reply);
    }
}
