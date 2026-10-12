#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"
#include "sdk/mi.h"

WmspSlot *Wmsp_TakeFlaggedSlot(void);

/*
 * WIFI_HOSTMSG_PEER_JOINED: a child joined this parent. While entry is closed the child
 * is deauthenticated again; otherwise it is recorded under its aid and the ARM9
 * gets START_PARENT/CONNECTED.
 */
void Wmsp_IndicateAssociate(WlAssocIndication *ind)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u16 aid = ind->aid;
    int irqState;
    volatile u16 seqNo; /* needed to match: the original passes it through a stack slot */
    WMStartParentCallback *callback;

    if (aid == 0 || aid >= 16) {
        return;
    }
    if (((WMParentParam *)status->pparam)->entryFlag == 0) {
        WmspAutoDeauthRequest *request = (WmspAutoDeauthRequest *)Wmsp_TakeFlaggedSlot();
        BOOL queued;

        if (request == NULL) {
            queued = FALSE;
        } else {
            request->apiid = WM_APIID_AUTO_DEAUTH;
            Mem_CopyBytes_Arm7(ind->macAddress, request->macAddress, 6);
            queued = MsgQueue_PushBack_Arm7(&func_arm7_027c05e0()->requestQueue, request, OS_MESSAGE_NOBLOCK);
        }
        if (!queued) {
            RadioNotice *error = Wmsp_GetCallbackBuffer();

            error->apiid = WM_APIID_INDICATION;
            error->errcode = WM_ERRCODE_FIFO_ERROR;
            error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
            error->reason = WM_APIID_AUTO_DEAUTH;
            Wmsp_SendReply(error);
        }
        return;
    }

    Mem_CopyBytes_Arm7(ind->macAddress, status->childMacAddress[aid - 1], 6);
    irqState = Cpu_MaskIrq_Arm7();
    status->childBitmap |= 1 << aid;
    status->mpReadyBitmap &= ~(1 << aid);
    status->mpLastRecvTick[aid] = Tick_Read64_Arm7() | 1;
    Cpu_RestoreIrqBit_Arm7(irqState);
    seqNo = 1;
    Mem_FillHalfwords_Arm7(seqNo, status->portSeqNo[aid], sizeof(status->portSeqNo[aid]));

    callback = Wmsp_GetCallbackBuffer();
    callback->apiid = WM_APIID_START_PARENT;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WM_STATECODE_CONNECTED;
    Mem_CopyBytes_Arm7(ind->macAddress, callback->macAddress, 6);
    callback->aid = aid;
    Mem_CopyHalfwords_Arm7(ind->ssid, callback->ssid, 24);
    Wmsp_SendReply(callback);
}
