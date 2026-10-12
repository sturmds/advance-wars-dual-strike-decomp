#include "sdk/wl.h"

void WifiTx_ResendQueueHead(WlTxQueue *queue);
void WifiTx_HaltAndSettle(void);
BOOL WifiTx_RestoreGuardWords(WlTxQueue *queue);
void WifiTx_BuildPsPoll(u32 aid);
void WifiBeacon_Build(void);

/*
 * Repairs queued frames whose marks were overwritten and restarts their
 * queues (WifiTx_ResendQueueHead). Queues 2, 5 and 6 depend on state.unk_24; in
 * mode 2 a damaged PS-Poll frame (queue 2) is built again. Register 0x004 is
 * set back to 1 if it was left at 0.
 */
void WifiTx_RepairDamagedQueues(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlTxQueue *queues = WIFI_SEND_QUEUE_RECORDS_OF(work);

    switch (work->state.unk_24) {
    case 1:
        if (WifiTx_RestoreGuardWords(&queues[6])) {
            WifiTx_HaltAndSettle();
            WifiBeacon_Build();
        }
        if (WifiTx_RestoreGuardWords(&queues[2])) {
            WifiTx_ResendQueueHead(&queues[2]);
        }
        break;
    case 2:
        WifiTx_RestoreGuardWords(&queues[5]);
        if (WifiTx_RestoreGuardWords(&queues[2])) {
            if (queues[2].busy != 0) {
                WifiTx_HaltAndSettle();
            }
            WifiTx_BuildPsPoll(link->aid);
            link->unk_b8++;
        }
        break;
    case 3: /* needed to match: the original compares with 3 */
        break;
    }
    if (WifiTx_RestoreGuardWords(&queues[1])) {
        WifiTx_ResendQueueHead(&queues[1]);
    }
    if (WifiTx_RestoreGuardWords(&queues[0])) {
        WifiTx_ResendQueueHead(&queues[0]);
    }
    if (WIFI_MAC_RUN == 0) {
        WIFI_MAC_RUN = 1;
    }
}
