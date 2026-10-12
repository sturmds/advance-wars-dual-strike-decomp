#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

u8 Wmsp_DecodeRssi(int rawRssi);
void Wmsp_WaitCallbackFree(void);
void Wmsp_PostMpAckTimeout(void *arg);
int WMSP_UpdateSendQueue(BOOL timedOut, u16 polledAgain);

/* WL indication WIFI_HOSTMSG_POLL_HEARD on a child: an MP frame from the parent arrived.
   Copies it into the next receive buffer, waits for the parent's ack with an
   alarm, and reports the frame to the ARM9. unk_ba keeps the lowest RSSI. */
void Wmsp_IndicateMp(WlMpIndication *ind)
{
    // Needed to match: this declaration order sets the register allocation.
    u32 size;
    BOOL bits6000;
    u32 rssi;
    u16 polled;
    WMStatus *status;
    RadioChildFrameBuffer *recvBuf;
    int intr;
    BOOL wasWaiting;
    WMStartMPCallback *reply;
    BOOL bits2800;
    u16 oldUnk_8e;

    status = func_arm7_027c05e0()->buf->status;

    rssi = Wmsp_DecodeRssi(ind->frame.rssi);
    if (status->unk_ba > rssi) {
        status->unk_ba = rssi;
    }
    if (status->mpFlag == 0) {
        return;
    }
    if (status->unk_32 == 1) {
        status->unk_32 = 0;
    }
    oldUnk_8e = status->unk_8e;
    status->mpRecvBufSel ^= 1;
    recvBuf = status->mpRecvBuf[status->mpRecvBufSel];
    size = ind->frame.length + 0x30;
    if (status->mpRecvBufSize < size) {
        size = status->mpRecvBufSize;
    }
    Mem_CopyBytes_Arm7(&ind->frame, recvBuf, size);

    intr = Cpu_MaskIrq_Arm7();
    wasWaiting = FALSE;
    if (status->mpWaitAckFlag == 1) {
        wasWaiting = TRUE;
        Alarm_Withdraw_Arm7(&status->mpAckAlarm);
    }
    status->mpWaitAckFlag = 1;
    status->unk_56 = recvBuf->ackTimeStamp;
    polled = 1;
    if ((recvBuf->pollBitmap & (1 << status->aid)) == 0) {
        polled = 0;
    }
    status->unk_90 = polled;
    // The ack is due (ackTimeStamp - timeStamp + 0x80) * 16 microseconds from
    // now; 0x82ea / 64 is the ARM7 timer rate in kHz (33.514 MHz / 64).
    Alarm_ArmOneShot_Arm7(&status->mpAckAlarm,
                     _s32_div_f(_s32_div_f((((u16)(recvBuf->ackTimeStamp - recvBuf->timeStamp) + 0x80) << 4) * 0x82ea, 64), 1024),
                     Wmsp_PostMpAckTimeout, NULL);
    bits2800 = (ind->frame.unk_08 & 0x2800) == 0x2800;
    bits6000 = (ind->frame.unk_08 & 0x6000) == 0x6000;
    if (bits2800) {
        status->unk_8a = 0;
    }
    status->unk_8e = bits2800 ? 1 : 0;
    status->unk_8c = bits6000 ? 1 : 0;
    Cpu_RestoreIrqBit_Arm7(intr);

    if (wasWaiting) {
        if (oldUnk_8e == 1) {
            WMSP_UpdateSendQueue(wasWaiting, 0);
        }
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_MP;
        reply->errcode = WM_ERRCODE_TIMEOUT;
        reply->state = WM_STATECODE_MPACK_IND;
        reply->recvBuf = NULL;
        Wmsp_SendReply(reply);
    }
    Mem_CopyBytes_Arm7(ind->frame.destAdrs, recvBuf->destAdrs, 6);
    Mem_CopyBytes_Arm7(ind->frame.srcAdrs, recvBuf->srcAdrs, 6);

    if (recvBuf->length >= 2) {
        recvBuf->length -= 2;
        status->unk_30 = (recvBuf->wmHeader & 0x8000) ? 1 : 0;
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_MP;
        if (polled) {
            reply->errcode = WM_ERRCODE_SUCCESS;
        } else {
            reply->errcode = WM_ERRCODE_INVALID_POLLBITMAP;
        }
        reply->state = WM_STATECODE_MP_IND;
        reply->recvBuf = recvBuf;
        Wmsp_SendReply(reply);
        if (recvBuf->length != 0) {
            WMSP_ParsePortPacket(0, recvBuf->wmHeader, recvBuf->data, recvBuf->length, recvBuf);
            Wmsp_WaitCallbackFree();
        }
    } else {
        recvBuf->length = 0;
        status->unk_30 = 0;
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_MP;
        if (polled) {
            reply->errcode = WM_ERRCODE_NO_DATA;
        } else {
            reply->errcode = WM_ERRCODE_INVALID_POLLBITMAP;
        }
        reply->state = WM_STATECODE_MP_IND;
        reply->recvBuf = recvBuf;
        Wmsp_SendReply(reply);
        Wmsp_WaitCallbackFree();
    }

    if (status->unk_7b0 != 0) {
        status->mpLastRecvTick[0] = Tick_Read64_Arm7() | 1;
    }
}
