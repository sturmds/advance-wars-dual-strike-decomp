#include "sdk/wmsp.h"

// Test mode is not built into this ARM7 side: the request always fails.
void WMSP_StopTestMode(void) {
  WmspReply* reply = Wmsp_GetCallbackBuffer();

  reply->apiid = WM_APIID_STOP_TESTMODE;
  reply->errcode = WM_ERRCODE_WM_DISABLE;
  Wmsp_SendReply(reply);
}
