#include "sdk/wmsp.h"

void WMSP_ReplySetParentParamFailed(u16 wlCmdID, u16 wlResult)
{
    WmspReply *reply = func_arm7_03807640();

    reply->apiid = WM_APIID_SET_P_PARAM;
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = wlCmdID;
    reply->wlResult = wlResult;
    func_arm7_038076e8(reply);
}
