#include "types.h"
#include "sdk/wmsp.h"

/* WIFI_HOSTMSG_PARENT_GONE: the child lost the parent's beacon. Reported only while
   WMStatus.unk_be is set. */
void Wmsp_IndicateBeaconLost(WlIndicationHeader *ind)
{
    WMStartConnectCallback *reply;

    if (func_arm7_027c05e0()->buf->status->unk_be != 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->state = WM_STATECODE_BEACON_LOST;
        Wmsp_SendReply(reply);
    }
}
