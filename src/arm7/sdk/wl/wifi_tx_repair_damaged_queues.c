#include "sdk/wl.h"

void WifiTx_ResendQueueHead(WlTxQueue *queue);
void func_arm7_027c8240(void);
BOOL func_arm7_027c8284(WlTxQueue *queue);
void WifiTx_BuildPsPoll(u32 aid);
void func_arm7_027cfd48(void);

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
        if (func_arm7_027c8284(&queues[6])) {
            func_arm7_027c8240();
            func_arm7_027cfd48();
        }
        if (func_arm7_027c8284(&queues[2])) {
            WifiTx_ResendQueueHead(&queues[2]);
        }
        break;
    case 2:
        func_arm7_027c8284(&queues[5]);
        if (func_arm7_027c8284(&queues[2])) {
            if (queues[2].busy != 0) {
                func_arm7_027c8240();
            }
            WifiTx_BuildPsPoll(link->aid);
            link->unk_b8++;
        }
        break;
    case 3: /* needed to match: the original compares with 3 */
        break;
    }
    if (func_arm7_027c8284(&queues[1])) {
        WifiTx_ResendQueueHead(&queues[1]);
    }
    if (func_arm7_027c8284(&queues[0])) {
        WifiTx_ResendQueueHead(&queues[0]);
    }
    if (WIFI_MAC_RUN == 0) {
        WIFI_MAC_RUN = 1;
    }
}
