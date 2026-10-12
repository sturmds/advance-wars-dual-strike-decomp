#include "types.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

/* WIFI_HOSTMSG_PEER_LEFT: tells the ARM9 a station disassociated. */
void Wmsp_IndicateDisassociate(WlIndicationHeader* ind) {
  RadioNotice* callback = Wmsp_GetCallbackBuffer();

  callback->apiid = WM_APIID_INDICATION;
  callback->errcode = WM_ERRCODE_SUCCESS;
  callback->state = WIRELESS_NOTICE_STATION_DROPPED;
  Wmsp_SendReply(callback);
}
