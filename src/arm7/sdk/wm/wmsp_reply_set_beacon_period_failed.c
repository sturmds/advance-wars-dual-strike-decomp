#include "sdk/wmsp.h"

void WMSP_ReplySetBeaconPeriodFailed(u16 wlCmdID, u16 wlResult)
{
    WmspReply *reply = Wmsp_GetCallbackBuffer();

    reply->apiid = WM_APIID_SET_BEACON_PERIOD;
    reply->errcode = WM_ERRCODE_FAILED;
    reply->wlCmdID = wlCmdID;
    reply->wlResult = wlResult;
    Wmsp_SendReply(reply);
}
