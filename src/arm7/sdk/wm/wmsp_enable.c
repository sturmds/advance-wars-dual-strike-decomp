#include "sdk/wmsp.h"

/* Takes the buffers the ARM9 passes, resets the status and leaves the
   wireless hardware off (WM_STATE_STOP). */
void WMSP_Enable(WmspInitRequest *request)
{
    WmspWork *work = func_arm7_027c05e0();
    WmspBuf *buf;
    WmspReply *reply;

    work->buf = request->arm7Buf;
    buf = work->buf;
    buf->status = request->status;
    buf->replyBuf = request->replyBuf;
    WMSP_ResetStatus();
    PowerLed_SelectPattern(15);
    buf->status->state = WM_STATE_STOP;
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_ENABLE;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
