#include "sdk/wl.h"

void WifiTx_RepairDamagedQueues(void);

/* Receive event counter interrupt. */
void WifiIrq_RxEventCounted(void)
{
    WlWepWatch *watch = &WIFI_DRIVER_STATE->wepWatch;
    WlMpReplyView *mp = WIFI_REPLY_SLOTS_VIEW_OF(WIFI_DRIVER_STATE);
    u16 events;

    WIFI_IRQ_PENDING_FLAGS = W_IRQ_RX_COUNT;
    events = REG_W_RXSTAT_INC_IF;
    if ((WIFI_DRIVER_STATE->unk_638 & 8) && (events & 0x400)) {
        u16 txreq = REG_W_TXREQ_READ;

        /* With no TX slot request pending and the carrier clear, restart
           the WEP engine at once; otherwise only after 13 such events. */
        if (!(((txreq & 1) && mp->txSlots[0].busy != 0) || ((txreq & 4) && mp->txSlots[1].busy != 0)
              || ((txreq & 8) && mp->txSlots[2].busy != 0) || (REG_W_RF_PINS & W_RF_PINS_CARRIER_SENSE))) {
            REG_W_WEP_CNT = 0;
            REG_W_WEP_CNT = 0x8000;
            watch->busyCount = 0;
        } else if (watch->busyCount++ > 12) {
            watch->busyCount = 0;
            REG_W_WEP_CNT = 0;
            REG_W_WEP_CNT = 0x8000;
            WIFI_DRIVER_STATE->state.wepRestarts++;
        }
    }
    if ((WIFI_DRIVER_STATE->unk_638 & 1) && (events & 0x60)) {
        u16 cursor = REG_W_RXBUF_WRCSR;

        /* The write cursor (a halfword offset) left the receive buffer
           (byte addresses from 0x4000): restart it at the read cursor. */
        if (cursor >= (s32)func_arm7_03806d4c(REG_W_RXBUF_END - 0x4000, 2)
            || cursor < (s32)func_arm7_03806d4c(REG_W_RXBUF_BEGIN - 0x4000, 2)) {
            REG_W_RXBUF_WR_ADDR = REG_W_RXBUF_READCSR;
            REG_W_RXCNT = 0x8001;
        }
        WifiTx_RepairDamagedQueues();
    }
}
