#include "sdk/wmsp.h"

/* Disconnects the children in request->arg and returns 1 on success; on a
   failure it sends the failed reply itself. */
int Wmsp_DisconnectCore(WmspRequest* request, int isAuto,
                        u16* disconnectedBitmap);

void WMSP_Disconnect(WmspRequest* request) {
  u16 tryBitmap = request->arg;
  u16 disconnectedBitmap;
  RadioDropReply* reply;

  if (Wmsp_DisconnectCore(request, 0, &disconnectedBitmap) == 1) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_DISCONNECT;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->tryBitmap = tryBitmap;
    reply->disconnectedBitmap = disconnectedBitmap;
    Wmsp_SendReply(reply);
  }
}
