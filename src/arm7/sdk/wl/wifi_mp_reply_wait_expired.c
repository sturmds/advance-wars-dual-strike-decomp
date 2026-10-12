#include "sdk/wl.h"

void func_arm7_037fa0b4(int arg);

// One-shot alarm set by the MP end interrupt handler (func_arm7_037fa0b4):
// pulses bit 12 of register 0x244, waits up to 100 reads for the hardware to
// send a frame, then runs the handler again without setting the alarm.
void WifiMp_ReplyWaitExpired(void *arg)
{
    OSIrqMask saved = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
    u16 seqNo = REG_W_TX_SEQNO;
    int tries;

    WIFI_MAC_REGISTER(0x244) = 0x1000;
    for (tries = 100; tries != 0; tries--) {
        if (seqNo != REG_W_TX_SEQNO) {
            break;
        }
    }
    WIFI_MAC_REGISTER(0x244) = 0;
    func_arm7_037fa0b4(0);
    Irq_SetEnableBits_Arm7(saved);
}
