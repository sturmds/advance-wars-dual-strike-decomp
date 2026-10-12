#include "sdk/wl.h"

void WifiTx_StartNextGroupcast(void);
void WifiTx_StartNextOfQueue(int queue);

// Task 20: enables the TX locations and starts sending what waits in the TX
// queues. While register 0x088 is 0, a waiting multicast frame goes alone on
// LOC3; otherwise LOC3 is enabled only when the station bit masks allow it.
void WifiTask_ResumeSending(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlBufferLists* lists = WIFI_LISTS_VIEW_OF(work);

  if (WIFI_MAC_REGISTER(0x088) == 0 && lists->multicastQueue.count != 0) {
    REG_W_TXREQ_SET = W_TXREQ_LOC3;
    WifiTx_StartNextGroupcast();
    return;
  }
  if ((work->stationBits.unk_02 & ~work->stationBits.unk_06) == 0) {
    REG_W_TXREQ_SET = W_TXREQ_LOC3;
    if (lists->multicastQueue.count != 0) {
      WifiTx_StartNextGroupcast();
    }
  }
  REG_W_TXREQ_SET = W_TXREQ_LOC1 | W_TXREQ_LOC2;
  if (lists->txQueues[1].count != 0) {
    WifiTx_StartNextOfQueue(1);
  }
  if (lists->txQueues[0].count != 0) {
    WifiTx_StartNextOfQueue(0);
  }
}
