#include "sdk/wmsp.h"

void WMSP_ReplyResetFailed(u16 wlCmdID, u16 wlResult)
{
    WmspReply *reply = Wmsp_GetCallbackBuffer();

    reply->apiid = WM_APIID_RESET;
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = wlCmdID;
    reply->wlResult = wlResult;
    func_arm7_038076e8(reply);
}
