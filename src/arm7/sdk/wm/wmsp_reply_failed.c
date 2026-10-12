#include "types.h"
#include "sdk/wmsp.h"

void Wmsp_ReplyFailed(int apiid, int wlCmdID, int wlResult)
{
    WmspReply *reply = Wmsp_GetCallbackBuffer();

    reply->apiid = apiid;
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = wlCmdID;
    reply->wlResult = wlResult;
    Wmsp_SendReply(reply);
}
