#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

extern OSMessageQueue data_arm7_027d6ca8;

BOOL func_arm7_037fc44c(OSMessageQueue *queue, void *message, int flags);

void Wmsp_ReceiveRequest(int tag, WmspRequest *request, BOOL error)
{
    WmspReply *reply;

    if (error) {
        return;
    }
    if (func_arm7_037fc44c(&data_arm7_027d6ca8, request, 0)) {
        return;
    }
    if (data_arm7_027d7c44.buf == NULL) {
        return;
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = request->apiid;
    reply->errcode = WM_ERRCODE_FIFO_ERROR;
    reply->wlCmdID = 0;
    reply->wlResult = 0;
    Wmsp_SendReply(reply);
}
