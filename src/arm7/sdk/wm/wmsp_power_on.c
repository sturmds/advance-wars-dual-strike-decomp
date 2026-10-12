#include "sdk/wmsp.h"

/* Only allowed in WM_STATE_STOP: starts the wireless firmware and moves to
   WM_STATE_IDLE. */
void WMSP_PowerOn(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WmspReply *reply;
    u16 wlCmdID;
    u16 wlResult;

    if (buf->status->state != WM_STATE_STOP) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_POWER_ON;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
    } else if (WMSP_InitWl(&wlCmdID, &wlResult) == FALSE) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_POWER_ON;
        reply->errcode = WM_ERRCODE_FAILED;
        reply->wlCmdID = wlCmdID;
        reply->wlResult = wlResult;
        Wmsp_SendReply(reply);
    } else {
        buf->status->state = WM_STATE_IDLE;
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_POWER_ON;
        reply->errcode = WM_ERRCODE_SUCCESS;
        Wmsp_SendReply(reply);
    }
}
