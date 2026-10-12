#include "sdk/wmsp.h"

u16 *func_arm7_027c1efc(WlCommand *cmd);
void WMSP_ReplyEndScanFailed(u16 wlCmdID, u16 wlResult);

/* Only allowed in WM_STATE_SCAN: sends the firmware back to idle (WL command
   0x302) and moves to WM_STATE_IDLE. */
void WMSP_EndScan(void)
{
    WlCommandBuf cmdBuf;
    WlCommand *cmd = &cmdBuf.command;
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    u16 *result;
    WmspReply *reply;

    if (buf->status->state != WM_STATE_SCAN) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_END_SCAN;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
        return;
    }
    result = func_arm7_027c1efc(cmd);
    if (result[2] != 0) {
        WMSP_ReplyEndScanFailed(0x302, result[2]);
        return;
    }
    buf->status->state = WM_STATE_IDLE;
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_END_SCAN;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
