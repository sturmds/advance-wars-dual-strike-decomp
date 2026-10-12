#include "sdk/wmsp.h"
#include "sdk/os_arm7.h"

void Wmsp_SetNormalThreadPriorities(void);
u16 *func_arm7_027c224c(WlCommand *cmd, u16 arg);
void func_arm7_027c632c(int aidBitmap);
void WMSP_ReplyEndMPFailed(u16 wlCmdID, u16 wlResult);

void WMSP_EndMP(void)
{
    WmspWork *work = func_arm7_027c05e0();
    WMStatus *status = work->buf->status;
    BOOL wasRunning = FALSE;
    u32 wlBuf[WMSP_WL_BUF_WORDS];
    WmspReply *reply;
    int irqState;
    u16 *wlReply;

    if (status->state != WM_STATE_MP_PARENT && status->state != WM_STATE_MP_CHILD) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_END_MP;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
        return;
    }

    irqState = Cpu_MaskIrq_Arm7();
    if (status->mpFlag == 1) {
        wasRunning = TRUE;
    }
    status->mpFlag = 0;
    LineAlarm_Withdraw_Arm7(&work->buf->vAlarm);
    Wmsp_SetNormalThreadPriorities();
    if (status->state == WM_STATE_MP_CHILD) {
        status->state = WM_STATE_CHILD;
    } else if (status->state == WM_STATE_MP_PARENT) {
        status->state = WM_STATE_PARENT;
    }
    Cpu_RestoreIrqBit_Arm7(irqState);

    wlReply = func_arm7_027c224c((WlCommand *)wlBuf, 7);
    if (wlReply[2] != 0) {
        WMSP_ReplyEndMPFailed(0x104, wlReply[2]);
        return;
    }
    if (wasRunning) {
        func_arm7_027c632c(0xffff);
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_END_MP;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
