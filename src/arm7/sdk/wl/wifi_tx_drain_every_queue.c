#include "sdk/wl.h"

/*
 * Empties the three TX queues. Which ones go back to the host depends on
 * WlState.unk_24; in mode 1 a pending CMD slot is freed too.
 */
void WifiTx_DrainEveryQueue(void)
{
    WlTxSlots *tx = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);

    switch (WIFI_DRIVER_STATE->state.unk_24) {
    case 1:
        WifiTx_DrainQueue(0, TRUE);
        WifiTx_DrainQueue(1, FALSE);
        WifiTx_DrainQueue(WIFI_SEND_QUEUE_GROUP, TRUE);
        if (tx->slot[WIFI_TXLOC_MP_POLL].busy) {
            tx->slot[WIFI_TXLOC_MP_POLL].busy = 0;
            tx->slot[WIFI_TXLOC_MP_POLL].count--;
            WifiBuffer_Release(&WIFI_DRIVER_STATE->unk_15c, tx->cmdBuffer);
        }
        break;
    case 2:
    case 3:
        WifiTx_DrainQueue(0, TRUE);
        WifiTx_DrainQueue(1, FALSE);
        WifiTx_DrainQueue(WIFI_SEND_QUEUE_GROUP, FALSE);
        break;
    default:
        WifiTx_DrainQueue(0, FALSE);
        WifiTx_DrainQueue(1, FALSE);
        WifiTx_DrainQueue(WIFI_SEND_QUEUE_GROUP, FALSE);
        break;
    }
}
