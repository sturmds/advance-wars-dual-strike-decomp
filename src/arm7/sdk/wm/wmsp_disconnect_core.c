#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

u16 *func_arm7_027c1efc(WlCommand *cmd);
u16 *func_arm7_027c2648(WlCommand *cmd, const void *src, int arg);
u16 *func_arm7_027c28f4(WlCommand *cmd, int arg);
void func_arm7_027c632c(u16 aidBitmap);
void Wmsp_SetNormalThreadPriorities(void);

/* Shared body of the DISCONNECT (isAuto 0) and AUTO_DISCONNECT (isAuto 1)
   requests. A child deauthenticates from its parent (WL command 5, reason 3)
   and goes back to WM_STATE_IDLE; a parent deauthenticates each connected
   child in request->arg. Stops MP on a child. Returns 1 and the stations
   it disconnected in *disconnectedBitmap (if not NULL); on a failure it
   sends the failed reply itself and returns 0. AUTO_DISCONNECT also reports
   each link it ended as a DISCONNECTED state with the request's reason. */
int Wmsp_DisconnectCore(WmspRequest *request, int isAuto, u16 *disconnectedBitmap)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    u16 tryBitmap = request->arg;
    u16 reason;
    u16 disconnected;
    BOOL mpRan;
    int irq;
    u16 *result;
    int aid;
    u16 aid16; /* needed to match: declared before mask */
    u32 mask;
    RadioDropReply *error;
    WMStartConnectCallback *childReply;
    WMStartParentCallback *parentReply;
    u8 parentMac[6];
    u8 childMac[6];
    WlCommandBuf cmd;

    reason = isAuto ? ((WmspAutoDisconnectRequest *)request)->reason : 0;
    disconnected = 0;
    mpRan = FALSE;

    if (buf->status->state == WM_STATE_MP_PARENT || buf->status->state == WM_STATE_PARENT) {
        if (buf->status->mpFlag == 1) {
            mpRan = TRUE;
        }
    } else if (buf->status->state == WM_STATE_MP_CHILD || buf->status->state == WM_STATE_CHILD) {
        irq = Cpu_MaskIrq_Arm7();
        if (buf->status->mpFlag == 1) {
            buf->status->mpFlag = 0;
            mpRan = TRUE;
            LineAlarm_Withdraw_Arm7(&buf->vAlarm);
            Wmsp_SetNormalThreadPriorities();
            if (buf->status->state == WM_STATE_MP_CHILD) {
                buf->status->state = WM_STATE_CHILD;
            }
        }
        buf->status->childBitmap = 0;
        buf->status->mpReadyBitmap = 0;
        buf->status->unk_14 = 0;
        buf->status->unk_10 = 0;
        buf->status->unk_1c = 0;
        Cpu_RestoreIrqBit_Arm7(irq);
    } else {
        if (!isAuto) {
            error = Wmsp_GetCallbackBuffer();
            error->apiid = WM_APIID_DISCONNECT;
            error->errcode = WM_ERRCODE_ILLEGAL_STATE;
            error->wlCmdID = 0;
            error->wlResult = 0;
            error->tryBitmap = tryBitmap;
            error->disconnectedBitmap = 0;
            Wmsp_SendReply(error);
        }
        return 0;
    }

    if (buf->status->state == WM_STATE_MP_CHILD || buf->status->state == WM_STATE_CHILD) {
        Mem_CopyBytes_Arm7(buf->status->parentMacAddress, parentMac, 6);
        func_arm7_027c2648(&cmd.command, parentMac, 3);
        disconnected = 1;
        buf->status->unk_be = 0;
        buf->status->state = WM_STATE_CLASS1;
        result = func_arm7_027c28f4(&cmd.command, 1);
        if (result[2] != 0) {
            if (isAuto) {
                WMSP_ReplyAutoDisconnectFailed(0, result[2], tryBitmap, disconnected);
            } else {
                WMSP_ReplyDisconnectFailed(0, result[2], tryBitmap, disconnected);
            }
            if (mpRan) {
                func_arm7_027c632c(1);
            }
            return 0;
        }
        result = func_arm7_027c1efc(&cmd.command);
        if (result[2] != 0) {
            if (isAuto) {
                WMSP_ReplyAutoDisconnectFailed(0x302, result[2], tryBitmap, disconnected);
            } else {
                WMSP_ReplyDisconnectFailed(0x302, result[2], tryBitmap, disconnected);
            }
            if (mpRan) {
                func_arm7_027c632c(1);
            }
            return 0;
        }
        buf->status->state = WM_STATE_IDLE;
        if (isAuto == TRUE) {
            childReply = Wmsp_GetCallbackBuffer();
            childReply->apiid = WM_APIID_START_CONNECT;
            childReply->errcode = WM_ERRCODE_SUCCESS;
            childReply->state = WM_STATECODE_DISCONNECTED;
            childReply->reason = reason;
            childReply->aid = buf->status->aid;
            buf->status->unk_18e = 0;
            buf->status->unk_190 = 0;
            Wmsp_SendReply(childReply);
        }
        if (mpRan) {
            func_arm7_027c632c(1);
        }
    } else {
        for (aid = 1; aid < 16; aid++) {
            WMStatus *status = buf->status;

            mask = 1 << aid;
            if (mask & (status->childBitmap & tryBitmap)) {
                aid16 = aid;
                Mem_CopyBytes_Arm7(status->childMacAddress[aid - 1], childMac, 6);
                func_arm7_027c2648(&cmd.command, childMac, 3);
                disconnected |= 1 << aid16;
                irq = Cpu_MaskIrq_Arm7();
                buf->status->childBitmap &= ~mask;
                buf->status->mpReadyBitmap &= ~mask;
                buf->status->mpLastRecvTick[aid16] = 0;
                Cpu_RestoreIrqBit_Arm7(irq);
                // Needed to match: the original indexes by aid, not aid - 1, here.
                (buf->status->childMacAddress - 1)[aid][0] = 0;
                (buf->status->childMacAddress - 1)[aid][1] = 0;
                (buf->status->childMacAddress - 1)[aid][2] = 0;
                (buf->status->childMacAddress - 1)[aid][3] = 0;
                (buf->status->childMacAddress - 1)[aid][4] = 0;
                (buf->status->childMacAddress - 1)[aid][5] = 0;
                if (isAuto == TRUE) {
                    parentReply = Wmsp_GetCallbackBuffer();
                    parentReply->apiid = WM_APIID_START_PARENT;
                    parentReply->errcode = WM_ERRCODE_SUCCESS;
                    parentReply->state = WM_STATECODE_DISCONNECTED;
                    parentReply->reason = reason;
                    parentReply->aid = aid16;
                    Mem_CopyBytes_Arm7(childMac, parentReply->macAddress, 6);
                    Wmsp_SendReply(parentReply);
                }
                if (mpRan) {
                    func_arm7_027c632c(mask);
                }
            }
        }
    }
    if (disconnectedBitmap != NULL) {
        *disconnectedBitmap = disconnected;
    }
    return 1;
}
