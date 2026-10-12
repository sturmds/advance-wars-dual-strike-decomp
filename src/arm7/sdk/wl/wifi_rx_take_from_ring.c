#include "sdk/wl.h"

// Copies a received frame out of the RX ring into a new buffer: the 12-byte
// hardware RX header at src and length bytes of 802.11 header and body.
// Returns NULL when no buffer is left.
WlRxFrame *WifiRx_TakeFromRing(const void *src, u32 length)
{
    WlRxBuffer *buffer = (WlRxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, length + 34);
    WlRxFrame *frame;

    if (buffer == NULL) {
        return NULL;
    }
    frame = &buffer->frame;
    WifiRx_CopyFromRingWrapped(&frame->unk_08, src, length + 12);
    frame->bodyLength = length - 24; /* minus the 802.11 header */
    /* rate in the low byte, the RSSI (low byte of unk_12) in the high byte */
    frame->rate = (frame->rate & 0xff) | ((u16)(frame->unk_12 & 0xff) << 8);
    return frame;
}
