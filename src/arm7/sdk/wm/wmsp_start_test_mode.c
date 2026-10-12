#include "sdk/wmsp.h"

/* This build has no test mode: the request always fails. */
void WMSP_StartTestMode(void)
{
    WmspReply *reply = Wmsp_GetCallbackBuffer();

    reply->apiid = WM_APIID_START_TESTMODE;
    reply->errcode = WM_ERRCODE_WM_DISABLE;
    Wmsp_SendReply(reply);
}
