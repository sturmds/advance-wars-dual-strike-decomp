#include "sdk/wl.h"

WlBuffer *func_arm7_037f8a5c(WlBuffer *buffer); /* returns buffer->next */
void WifiStation_UncountQueuedFrame(WlTxFrame *frame);
void WifiHost_ReturnSentFrame(WlBufferList *list, WlTxRequest *request);

/*
 * Marks every frame on a TX queue as not sent and takes it off its station's
 * count (the multicast queue has none). With toHost the frames go back to the
 * host.
 */
void WifiTx_DrainQueue(int queue, BOOL toHost)
{
    WlBuffer *buffer;
    WlBuffer *next;
    WlTxFrame *frame;

    /* needed to match: the test before a do-while, not a plain while */
    buffer = WIFI_SEND_QUEUE_OF(WIFI_DRIVER_STATE, queue)->first;
    if (buffer == WIFI_LIST_END) {
        return;
    }
    do {
        next = func_arm7_037f8a5c(buffer);
        frame = &((WlTxRequest *)buffer)->frame;
        if (queue != WIFI_SEND_QUEUE_GROUP) {
            WifiStation_UncountQueuedFrame(frame);
        }
        frame->result = WIFI_SEND_RESULT_UNSENT;
        if (toHost) {
            WifiHost_ReturnSentFrame(WIFI_SEND_QUEUE_OF(WIFI_DRIVER_STATE, queue), (WlTxRequest *)buffer);
        }
        buffer = next;
    } while (next != WIFI_LIST_END);
}
