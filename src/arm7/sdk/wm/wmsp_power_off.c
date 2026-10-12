#include "sdk/wmsp.h"

u16 *func_arm7_027c1f14(WlCommand *cmd);

/* Only allowed in WM_STATE_IDLE: stops the wireless firmware (WL command
   0x301) and moves to WM_STATE_STOP. */
void WMSP_PowerOff(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    u32 cmd[0x80];
    u16 *result;
    WmspReply *reply;

    if (buf->status->state != WM_STATE_IDLE) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_POWER_OFF;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
        return;
    }
    result = func_arm7_027c1f14((WlCommand *)cmd);
    if (result[2] != 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_POWER_OFF;
        reply->errcode = WM_ERRCODE_FAILED;
        reply->wlCmdID = 0x301;
        reply->wlResult = result[2];
        Wmsp_SendReply(reply);
        return;
    }
    buf->status->state = WM_STATE_STOP;
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_POWER_OFF;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
