#include "types.h"
#include "sdk/wmsp.h"

void Wmsp_ReplyFailed(int apiid, int wlCmdID, int wlResult)
{
    WmspReply *reply = func_arm7_03807640();

    reply->apiid = apiid;
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = wlCmdID;
    reply->wlResult = wlResult;
    func_arm7_038076e8(reply);
}
