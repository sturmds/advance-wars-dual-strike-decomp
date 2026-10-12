#include "sdk/wmsp.h"
#include "sdk/mi.h"

void WMSP_SetWepKey(WmspSetWepKeyRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
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

    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_WEPKEY;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
