#include "sdk/wmsp.h"

u16 *func_arm7_027c2004(WlCommand *cmd, u16 arg);
void WMSP_ReplySetBeaconPeriodFailed(u16 wlCmdID, u16 wlResult);

/* Request slot 36: sends the new beacon period with WL command 0x242. */
void WMSP_SetBeaconPeriod(WmspRequest *request)
{
    u32 cmd[0x80];
    u16 *result;
    WmspReply *reply;

    func_arm7_027c05e0();                       /* result unused, as in the original */
    result = func_arm7_027c2004((WlCommand *)cmd, request->arg);
    if (result[2] != 0) {
        WMSP_ReplySetBeaconPeriodFailed(0x242, result[2]);
        return;
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_BEACON_PERIOD;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
