#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

void Mem_CopyBytes_Arm7(const void *src, void *dest, u32 size);
void Alarm_Withdraw_Arm7(OSAlarm *alarm);
WmspSlot *Wmsp_TakeFlaggedSlot(void);
int func_arm7_027c6510(BOOL timedOut, u16 polledAgain);

/*
 * WIFI_HOSTMSG_POLL_ACKED: the ack that ends an MP exchange, or one Wmsp_PostMpAckTimeout
 * made up (unk_0e 0) once the ack deadline passed. Stops the ack alarm, reports
 * START_MP/MPACK_IND with the outcome, then queues request 41 or marks unk_32.
 */
void Wmsp_IndicateMpAck(WlMpAckIndication *ind)
{
    WMStartMPCallback *callback; /* needed to match: declared first */
    WlMpAckFrame *frame = &ind->frame;
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    BOOL timedOut;
    int result = 0;
    int irqState;
    u16 wasPolled;
    BOOL polledAgain;

    if (status->mpFlag == 0) {
        return;
    }
    if (ind->ind.unk_0e == 0) {
        u16 low;
        u16 high;
        u16 low2;
        u16 now;

        timedOut = TRUE;
        low = IO_WIRELESS_USEC_LOW;
        high = IO_WIRELESS_USEC_HIGH;
        low2 = IO_WIRELESS_USEC_LOW;
        if (low > low2) {
            high = IO_WIRELESS_USEC_HIGH;
        }
        now = (high << 12) | (low2 >> 4);
        if ((s16)(now - status->unk_56) <= 0) {
            return;
        }
    } else {
        timedOut = FALSE;
    }

    irqState = Cpu_MaskIrq_Arm7();
    if (status->mpWaitAckFlag == 0) {
        Cpu_RestoreIrqBit_Arm7(irqState);
        return;
    }
    status->mpWaitAckFlag = 0;
    wasPolled = status->unk_90;
    Alarm_Withdraw_Arm7((OSAlarm *)status->unk_58); /* the MP ack alarm; wm.h types it as bytes */
    Cpu_RestoreIrqBit_Arm7(irqState);

    polledAgain = TRUE;
    if (status->unk_8c != 0) {
        if (timedOut || !(frame->pollbmp & (1 << status->aid))) {
            polledAgain = FALSE;
        }
    }
    if (wasPolled != 0 && (timedOut || polledAgain)) {
        status->unk_ba = 4;
    }
    if (status->unk_8c != 0) {
        status->unk_8c = 0;
    }
    if (status->unk_8e != 0) {
        status->unk_8e = 0;
        result = func_arm7_027c6510(timedOut, polledAgain ? 1 : 0);
    }

    callback = Wmsp_GetCallbackBuffer();
    callback->apiid = WM_APIID_START_MP;
    if (wasPolled == 0) {
        callback->errcode = WM_ERRCODE_INVALID_POLLBITMAP;
    } else if (timedOut) {
        callback->errcode = WM_ERRCODE_TIMEOUT;
    } else if (frame->pollbmp & (1 << status->aid)) {
        callback->errcode = WM_ERRCODE_SEND_FAILED;
    } else {
        callback->errcode = WM_ERRCODE_SUCCESS;
    }
    callback->state = WM_STATECODE_MPACK_IND;
    callback->recvBuf = NULL;
    if (!timedOut) {
        callback->timeStamp = frame->timeStamp;
        callback->rate_rssi = frame->rate_rssi;
        Mem_CopyBytes_Arm7(frame->destAdrs, callback->destAdrs, 6);
        Mem_CopyBytes_Arm7(frame->srcAdrs, callback->srcAdrs, 6);
        callback->seqNum = frame->seqNum;
        callback->tmptt = frame->tmptt;
        callback->pollbmp = frame->pollbmp;
    }
    Wmsp_SendReply(callback);

    if (wasPolled == 0) {
        return;
    }
    if (result == 1 || status->unk_30 == 0) {
        WmspInternalRequest *request = (WmspInternalRequest *)Wmsp_TakeFlaggedSlot();
        WmspWork *work = func_arm7_027c05e0();
        BOOL queued = FALSE;

        status->unk_30 = 0;
        status->unk_32 = 0;
        status->unk_88 = 0;
        if (request != NULL) {
            request->apiid = 41; /* not identified; the API id numbering is unclear around here */
            queued = MsgQueue_PushBack_Arm7(&work->requestQueue, request, OS_MESSAGE_NOBLOCK);
        }
        if (!queued && work->buf != NULL) {
            RadioNotice *error = Wmsp_GetCallbackBuffer();

            error->apiid = WM_APIID_INDICATION;
            error->errcode = WM_ERRCODE_FIFO_ERROR;
            error->state = WIRELESS_NOTICE_QUEUE_REFUSED;
            error->reason = 41;
            Wmsp_SendReply(error);
        }
    } else {
        status->unk_30 = 0;
        status->unk_32 = 1;
        status->unk_88 = 0;
    }
}
