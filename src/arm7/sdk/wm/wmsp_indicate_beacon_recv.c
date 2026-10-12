#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

WmspSlot *Wmsp_TakeFlaggedSlot(void);

/* WIFI_HOSTMSG_BEACON_HEARD on a child: a beacon from the parent arrived. If the
   parent's tgid changed, queues an auto-disconnect from the parent; otherwise
   passes the beacon's game info to the ARM9 while WMStatus.unk_be is set. */
void Wmsp_IndicateBeaconRecv(WlBeaconIndication *ind)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMGameInfo *gameInfo = &ind->gameInfo;
    WMStatus *status = buf->status;
    WmspAutoDisconnectRequest *request;
    BOOL queued;
    RadioNotice *error;
    RadioHeardBeaconNotice *reply;

    if (status->state != WM_STATE_CHILD && status->state != WM_STATE_MP_CHILD) {
        return;
    }
    if (status->tgid != gameInfo->tgid) {
        request = (WmspAutoDisconnectRequest *)Wmsp_TakeFlaggedSlot();
        if (request == NULL) {
            queued = FALSE;
        } else {
            request->apiid = WM_APIID_AUTO_DISCONNECT;
            request->aidBitmap = 1; /* the parent */
            request->reason = 0x8002; /* reason code for a changed tgid, a guess */
            queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue, request, OS_MESSAGE_NOBLOCK);
        }
        if (!queued) {
            error = Wmsp_GetCallbackBuffer();
            error->apiid = WM_APIID_INDICATION;
            error->errcode = WM_ERRCODE_FIFO_ERROR;
            error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
            error->reason = WM_APIID_AUTO_DISCONNECT;
            Wmsp_SendReply(error);
        }
    } else if (func_arm7_027c05e0()->buf->status->unk_be != 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_INDICATION;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->state = WIRELESS_NOTICE_HEARD_BEACON;
        reply->tgid = gameInfo->tgid;
        reply->wmstate = buf->status->state;
        reply->gameInfoLength = ind->gameInfoLength;
        if (reply->gameInfoLength <= sizeof(WMGameInfo)) {
            Mem_CopyHalfwords_Arm7(gameInfo, &reply->gameInfo, (reply->gameInfoLength + 1) & ~1);
        }
        Wmsp_SendReply(reply);
    }
}
