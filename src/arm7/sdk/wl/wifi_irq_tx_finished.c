#include "sdk/wl.h"

void WifiTest_ResendOrHalt(void);
void WifiIrq_MpCommandDone(int arg);
void WifiTx_PauseDataSlots(void);

/* Transmit done interrupt. Most kinds end by restarting the MP command
   transmit request; every kind queues task 14. */
void WifiIrq_TxFinished(void)
{
    WlTxSlotView *mp = WIFI_TX_SLOT_RECORDS_OF(WIFI_DRIVER_STATE);
    u32 kind;

    WIFI_IRQ_PENDING_FLAGS = W_IRQ_TX_DONE;
    if (WIFI_DRIVER_STATE->state.mode == WIFI_MODE_FRAME_TEST) {
        WifiTest_ResendOrHalt();
        return;
    }
    kind = REG_W_TXSTAT & W_TXSTAT_KIND;
    switch (kind) {
    case W_TXSTAT_LOC3:
        if (mp->unk_8c & 2) {
            WifiTx_PauseDataSlots();
        }
        WIFI_DRIVER_STATE->counters[7]++;
        WifiTask_Schedule(0, 8);
        break;
    case W_TXSTAT_MP_CMD:
        if (mp->unk_9e <= (u16)(mp->txSlots[3].frame->unk_04 & 0xff)) {
            REG_W_TXREQ_RESET = 2;
            mp->unk_aa++;
        }
        mp->mpBuffer->unk_18++;
        break;
    case W_TXSTAT_MP_REPLY: {
        u16 expected = mp->unk_98;
        u16 polled = mp->txSlots[3].frame->unk_02;
        u8 *entry = mp->mpBuffer->entries;

        if (polled > 1 && WIFI_DRIVER_STATE->state.flags.bit4) {
            if (!(REG_W_RF_PINS & W_RF_PINS_CARRIER_SENSE)) {
                WIFI_ANTENNA_SELECT ^= 1;
            }
        }
        if (WIFI_DRIVER_STATE->unk_638 & 0x40) {
            /* Bit n of polled, n >= 1, is child n; children with a bit in
               expected have an entry in mpBuffer, in bit order. */
            while (polled > 1) {
                polled >>= 1;
                if (polled & 1) {
                    ((WlMpChildEntry *)entry)->unk_06++;
                }
                expected >>= 1;
                if (expected & 1) {
                    entry += mp->mpBuffer->entrySize;
                }
            }
        }
        break;
    }
    }

    if (kind != W_TXSTAT_MP_CMD && !(REG_W_TXREQ_READ & 2)) {
        if (mp->txSlots[3].busy != 0) {
            WIFI_MAC_REGISTER(0x0b4) = 2;
            WIFI_MAC_POWER_AUX = 0;
            if (WIFI_IRQ_PENDING_FLAGS & W_IRQ_MP_END) {
                mp->unk_ae++;
            } else {
                WifiIrq_MpCommandDone(2);
            }
            mp->unk_ac++;
        }
        REG_W_TXREQ_SET = 2;
    }
    WifiTask_Schedule(0, 14);
}
