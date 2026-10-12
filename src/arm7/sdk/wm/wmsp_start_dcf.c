#include "sdk/wmsp.h"

void WMSP_StartDCF(WmspStartDCFRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    void *recvBuf = request->recvBuf;
    u16 recvBufSize = request->recvBufSize;
    int irqState = Cpu_MaskIrq_Arm7();
    RadioDcfReply *reply;

    status->dcfRecvBuf[0] = recvBuf;
    status->dcfRecvBufSize = recvBufSize;
    status->dcfRecvBuf[1] = (u8 *)recvBuf + recvBufSize;
    status->dcfRecvBufSel = 0;
    status->dcfSendData = NULL;
    status->dcfSendSize = 0;
    status->dcfSendFlag = 0;
    status->state = WM_STATE_DCF_CHILD;

    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_DCF;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WIRELESS_NOTICE_DCF_BEGUN;
    Wmsp_SendReply(reply);

    status->unk_10 = 1;
    Cpu_RestoreIrqBit_Arm7(irqState);
}
