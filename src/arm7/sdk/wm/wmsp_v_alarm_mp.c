#include "sdk/wmsp.h"

void Wmsp_PushRssiSample(u8 rssi);
int Wmsp_GetLinkLevel(void);

#define RSSI_NONE 0xffff

/*
 * Request slot 28, which the V-count alarm queues once a frame during MP. Every
 * fourth call turns the held RSSI sample into a link level. A parent adds unk_34
 * to unk_36 (at most 256) and starts an MP send when unk_36 leaves 0; a child
 * sends its MP data when an MP ack marked unk_32.
 */
void WMSP_VAlarmMp(WmspRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    BOOL send;
    BOOL wasEmpty;
    int irqState;

    status->unk_ca = 0;
    status->unk_88 = 1;

    status->unk_bc--;
    if (status->unk_bc == 0) {
        if (status->unk_ba != RSSI_NONE) {
            Wmsp_PushRssiSample(status->unk_ba & 0xff);
            status->linkLevel = Wmsp_GetLinkLevel();
            status->unk_ba = RSSI_NONE;
        }
        status->unk_bc = 4;
    }

    if (status->state == WM_STATE_MP_PARENT) {
        irqState = Cpu_MaskIrq_Arm7();
        wasEmpty = status->unk_36 == 0;
        status->unk_36 += status->unk_34;
        if (status->unk_36 > 256) {
            status->unk_36 = 256;
        }
        send = wasEmpty && status->unk_36 != 0;
        Cpu_RestoreIrqBit_Arm7(irqState);
        if (send) {
            func_arm7_027c6f68(0xffff);
        }
        if (status->unk_92 == 1) {
            status->unk_9c--;
            if (status->unk_9c == 0) {
                status->unk_9a = 1;
                status->unk_9c = 60;
            }
        }
    } else if (status->state == WM_STATE_MP_CHILD) {
        send = FALSE;
        irqState = Cpu_MaskIrq_Arm7();
        if (status->unk_32 == 1 && status->unk_72c != 1) {
            send = TRUE;
            status->unk_32 = 0;
        }
        Cpu_RestoreIrqBit_Arm7(irqState);
        if (send == TRUE) {
            func_arm7_027c70f8();
        }
    }
}
