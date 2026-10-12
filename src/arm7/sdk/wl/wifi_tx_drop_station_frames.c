#include "sdk/wl.h"

WlBuffer* func_arm7_037f8a5c(WlBuffer* buffer); /* returns buffer->next */
void WifiStation_UncountQueuedFrame(WlTxFrame* frame);
void WifiHost_ReturnSentFrame(WlBufferList* list, WlTxRequest* request);

/*
 * Takes a station's frames off TX queues 0 and 2 (queue 1 is left alone),
 * except the one its slot is sending, and hands them back to the host.
 */
void WifiTx_DropStationFrames(u32 station) {
  WlBuffer* buffer;
  WlBuffer* next;
  WlTxFrame* frame;
  BOOL found; /* set and never read, as in the original */
  u32 queue;

  found = FALSE;
  if (WifiStation_PendingTxOf(station) == 0) {
    return;
  }
  for (queue = 0; queue < WIFI_SEND_QUEUE_TOTAL; queue += 2) {
    buffer = WIFI_SEND_QUEUE_OF(WIFI_DRIVER_STATE, queue)->first;
    if (buffer == WIFI_LIST_END) {
      continue;
    }
    do {
      next = func_arm7_037f8a5c(buffer);
      frame = &((WlTxRequest*)buffer)->frame;
      if (frame->station == station &&
          frame !=
              WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[queue].queued) {
        frame->result = WIFI_SEND_RESULT_UNSENT;
        WifiStation_UncountQueuedFrame(frame);
        WifiHost_ReturnSentFrame(WIFI_SEND_QUEUE_OF(WIFI_DRIVER_STATE, queue),
                                 (WlTxRequest*)buffer);
        if (!found) {
          found = TRUE;
        }
      }
      buffer = next;
    } while (buffer != WIFI_LIST_END);
  }
}
