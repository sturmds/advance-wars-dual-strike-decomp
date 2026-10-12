#include "types.h"
#include "sdk/wmsp.h"

/* WIFI_HOSTMSG_BEACON_OUT: the parent sent a beacon. Reported only while
   WMStatus.unk_be is set. */
void Wmsp_IndicateBeaconSent(WlIndicationHeader *ind)
{
    WMStartParentCallback *reply;

    if (func_arm7_027c05e0()->buf->status->unk_be != 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_PARENT;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->state = WM_STATECODE_BEACON_SENT;
        Wmsp_SendReply(reply);
    }
}
