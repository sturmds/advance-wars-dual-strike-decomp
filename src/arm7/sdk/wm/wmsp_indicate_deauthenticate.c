#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

void Mem_CopyBytes_Arm7(const void *src, void *dest, u32 size);
void LineAlarm_Withdraw_Arm7(OSVAlarm *alarm);
void Wmsp_SetNormalThreadPriorities(void);
void func_arm7_027c632c(u16 aidBitmap);

/*
 * WIFI_HOSTMSG_PEER_UNAUTHED: a station left. A parent forgets that child and tells
 * the ARM9 which aid left; a child has lost its parent and goes back to CLASS1.
 */
void Wmsp_IndicateDeauthenticate(WlDeauthIndication *ind)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status = buf->status;

    if (status->state == WM_STATE_PARENT || status->state == WM_STATE_MP_PARENT) {
        u8 mac[6];
        u16 aid;
        int i;

        Mem_CopyBytes_Arm7(ind->macAddress, mac, 6);
        aid = 0;
        for (i = 0; i < 15; i++) {
            if (mac[0] == status->childMacAddress[i][0] && mac[1] == status->childMacAddress[i][1] &&
                mac[2] == status->childMacAddress[i][2] && mac[3] == status->childMacAddress[i][3] &&
                mac[4] == status->childMacAddress[i][4] && mac[5] == status->childMacAddress[i][5]) {
                int irqState;

                aid = i + 1;
                irqState = Cpu_MaskIrq_Arm7();
                status->childBitmap &= ~(1 << aid);
                status->mpReadyBitmap &= ~(1 << aid);
                status->mpLastRecvTick[aid] = 0;
                Cpu_RestoreIrqBit_Arm7(irqState);
                status->childMacAddress[i][0] = 0;
                status->childMacAddress[i][1] = 0;
                status->childMacAddress[i][2] = 0;
                status->childMacAddress[i][3] = 0;
                status->childMacAddress[i][4] = 0;
                status->childMacAddress[i][5] = 0;
                break;
            }
        }
        if (aid != 0) {
            WMStartParentCallback *callback = Wmsp_GetCallbackBuffer();

            callback->apiid = WM_APIID_START_PARENT;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_DISCONNECTED;
            callback->reason = ind->reason;
            callback->aid = aid;
            Mem_CopyBytes_Arm7(ind->macAddress, callback->macAddress, 6);
            Wmsp_SendReply(callback);
            func_arm7_027c632c(1 << aid);
        }
    } else {
        WMStartConnectCallback *callback;
        BOOL wasMp = FALSE;
        int irqState = Cpu_MaskIrq_Arm7();

        if (status->mpFlag == 1) {
            status->mpFlag = 0;
            wasMp = TRUE;
            LineAlarm_Withdraw_Arm7((OSVAlarm *)buf->unk_d0);
            Wmsp_SetNormalThreadPriorities();
        }
        status->childBitmap = 0;
        status->mpReadyBitmap = 0;
        status->unk_14 = 0;
        status->unk_10 = 0;
        status->unk_1c = 0;
        status->unk_be = 0;
        status->state = WM_STATE_CLASS1;
        Cpu_RestoreIrqBit_Arm7(irqState);

        callback = Wmsp_GetCallbackBuffer();
        callback->apiid = WM_APIID_START_CONNECT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->state = WM_STATECODE_DISCONNECTED;
        callback->reason = ind->reason;
        callback->aid = status->aid;
        status->unk_18e = 0;
        status->unk_190 = 0;
        Wmsp_SendReply(callback);
        if (wasMp) {
            func_arm7_027c632c(1);
        }
    }
}
