#include "types.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

/* WIFI_HOSTMSG_PEER_REJOINED: tells the ARM9 a station reassociated. */
void Wmsp_IndicateReassociate(WlIndication *ind)
{
    RadioNotice *callback = Wmsp_GetCallbackBuffer();

    callback->apiid = WM_APIID_INDICATION;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WIRELESS_NOTICE_STATION_BACK;
    Wmsp_SendReply(callback);
}
