#include "sdk/wmsp.h"

void WMSP_Initialize(WmspInitRequest *request)
{
    WmspWork *work = func_arm7_027c05e0();
    WmspBuf *buf;
    WmspReply *reply;
    u16 wlCmdID;
    u16 wlResult;

    work->buf = request->arm7Buf;
    buf = work->buf;
    buf->status = request->status;
    buf->replyBuf = request->replyBuf;
    func_arm7_027c73a4();
    func_arm7_03805688(15);
    buf->status->state = WM_STATE_STOP;
    if (func_arm7_027c75fc(&wlCmdID, &wlResult) == 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_INITIALIZE;
        reply->errcode = WM_ERRCODE_FAILED;
        reply->wlCmdID = wlCmdID;
        reply->wlResult = wlResult;
        Wmsp_SendReply(reply);
    } else {
        buf->status->state = WM_STATE_IDLE;
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_INITIALIZE;
        reply->errcode = WM_ERRCODE_SUCCESS;
        Wmsp_SendReply(reply);
    }
}
