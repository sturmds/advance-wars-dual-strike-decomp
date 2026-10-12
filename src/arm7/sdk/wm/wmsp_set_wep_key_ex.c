#include "sdk/wmsp.h"
#include "sdk/mi.h"

u16 *func_arm7_027c2190(WlCommand *cmd, u16 arg);
void WMSP_ReplySetWepKeyFailed(u16 wlCmdID, u16 wlResult);

void WMSP_SetWepKeyEx(WmspSetWepKeyRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u32 wlBuf[WMSP_WL_BUF_WORDS];
    u16 *wlReply;
    WmspReply *reply;

    status->wepMode = request->wepMode;
    switch (status->wepMode) {
    case WM_WEPMODE_NO:
        status->wepFlag = 0;
        break;
    case WIRELESS_CIPHER_KEY5:
    case WIRELESS_CIPHER_KEY13:
    case WIRELESS_CIPHER_KEY16:
        status->wepFlag = 1;
        break;
    default:
        status->wepFlag = 0;
        break;
    }
    Mem_CopyBytes_Arm7(request->wepKey, status->wepKey, sizeof(status->wepKey));
    status->wepKeyId = request->keyId;

    wlReply = func_arm7_027c2190((WlCommand *)wlBuf, status->wepKeyId);
    /* The failure goes out under SET_WEPKEY, and the success reply follows anyway. */
    if (wlReply[2] != 0) {
        WMSP_ReplySetWepKeyFailed(0x207, wlReply[2]);
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_WEPKEY_EX;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
