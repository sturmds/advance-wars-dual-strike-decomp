#include "types.h"
#include "sdk/wl.h"

void WifiTx_StartNextOfQueue(
    int queue); /* sends what waits in WlWork.txQueues[queue] */

/*
 * PS-Poll from an associated station: marks the station and sends what
 * waits in TX queues 1 and 0.
 */
void WifiParent_ServePsPoll(WlRxFrame* frame) {
  WlBufferLists* lists = WIFI_LISTS_VIEW_OF(WIFI_DRIVER_STATE);
  u16 station = frame->station;

  if (WifiStation_StateOf(station) == WIFI_PEER_ASSOCIATED) {
    func_arm7_027cb300(station);
    if (lists->txQueues[1].count != 0) {
      WifiTx_StartNextOfQueue(1);
    }
    if (lists->txQueues[0].count != 0) {
      WifiTx_StartNextOfQueue(0);
    }
  }
}
