#include "sdk/wmsp.h"

/* Only allowed with the wireless hardware off (WM_STATE_STOP): turns the LED
   steady and goes back to WM_STATE_READY. */
void WMSP_Disable(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WmspReply *reply;

    if (buf->status->state != WM_STATE_STOP) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_DISABLE;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
    } else {
        PowerLed_SelectPattern(1);
        buf->status->state = WM_STATE_READY;
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_DISABLE;
        reply->errcode = WM_ERRCODE_SUCCESS;
        Wmsp_SendReply(reply);
    }
}
