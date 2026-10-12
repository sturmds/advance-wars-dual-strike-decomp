#include "sdk/wmsp.h"

void Mem_CopyBytes_Arm7(const void *src, void *dest, u32 size); /* copy bytes */
u16 *func_arm7_027c2648(WlCommand *cmd, const void *src, int arg);

/* Deauthenticates the station with the given MAC address (WL command 5,
   reason 19), then replies WM_APIID_AUTO_DEAUTH. */
void WMSP_AutoDeAuth(WmspMacRequest *request)
{
    u8 macAddress[6];
    u32 cmd[0x80];
    u16 *result;
    WmspReply *reply;

    Mem_CopyBytes_Arm7(request->macAddress, macAddress, sizeof(macAddress));
    result = func_arm7_027c2648((WlCommand *)cmd, macAddress, 19);
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_AUTO_DEAUTH;
    if (result[2] == 0) {
        reply->errcode = WM_ERRCODE_SUCCESS;
    } else {
        reply->errcode = WM_ERRCODE_FAILED;
        reply->wlCmdID = 5;
        reply->wlResult = result[2];
    }
    Wmsp_SendReply(reply);
}
