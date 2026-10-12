#include "sdk/wl.h"

u32 WifiStation_IndexOfMac(const u16 *macAddress);
void WifiStation_CountQueuedFrame(WlTxFrame *frame);
void WifiTx_StartNextOfQueue(int queue);

/*
 * Queues a frame on TX queue 1 for the station it is addressed to (station 0
 * when no station has the address). A protected frame grows by 8 bytes.
 */
void WifiTx_EnqueueMgmt(WlTxFrame *frame)
{
    frame->station = WifiStation_IndexOfMac(frame->addr1);
    if (frame->station == WIFI_NO_PEER) {
        frame->station = 0;
    }
    frame->unk_04 = WIFI_DRIVER_STATE->state.periodicCount;
    if (frame->frameControl & WIFI_FRAMECTL_WEP_ON) {
        frame->unk_12 += 8;
    }
    WifiStation_CountQueuedFrame(frame);
    WifiBuffer_Transfer(&WIFI_DRIVER_STATE->unk_15c, &WIFI_DRIVER_STATE->txQueues[1], &WIFI_HOST_TX_REQUEST_OF(frame)->header);
    WifiTx_StartNextOfQueue(1);
}
