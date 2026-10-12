#include "sdk/wmsp.h"

/* Disconnects the children in request->arg and returns 1 on success; on a
   failure it sends the failed reply itself. arg1 is 0 here; nonzero probably
   means AUTO_DISCONNECT (it reads a second request word). */
int func_arm7_027c42c0(WmspRequest *request, int arg1, u16 *disconnectedBitmap);

void WMSP_Disconnect(WmspRequest *request)
{
    u16 tryBitmap = request->arg;
    u16 disconnectedBitmap;
    RadioDropReply *reply;

    if (func_arm7_027c42c0(request, 0, &disconnectedBitmap) == 1) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_DISCONNECT;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->tryBitmap = tryBitmap;
        reply->disconnectedBitmap = disconnectedBitmap;
        Wmsp_SendReply(reply);
    }
}
