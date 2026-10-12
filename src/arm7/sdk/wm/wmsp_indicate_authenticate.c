#include "types.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

/* WIFI_HOSTMSG_PEER_AUTHED: tells the ARM9 a station authenticated. */
void Wmsp_IndicateAuthenticate(WlIndicationHeader* ind) {
  RadioNotice* callback = Wmsp_GetCallbackBuffer();

  callback->apiid = WM_APIID_INDICATION;
  callback->errcode = WM_ERRCODE_SUCCESS;
  callback->state = WIRELESS_NOTICE_AUTHED;
  Wmsp_SendReply(callback);
}
