#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);
u32 WifiFrame_PutSsidElement(u8 *dest);
u32 WifiFrame_PutRatesElement(u8 *dest);

// Allocates a buffer and builds a probe request to dest with the SSID and
// rates elements. Returns NULL when no buffer is free.
WlTxFrame *WifiFrame_NewProbeAsk(const u16 *dest)
{
    WlTxBuffer *buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 90);
    WlTxFrame *frame;
    u32 length;

    if (buffer == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
    frame = &buffer->frame;
    WifiTx_FillHeader(frame, dest);
    length = WifiFrame_PutSsidElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->elements);
    length += WifiFrame_PutRatesElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->elements + length);
    frame->bodyLength = length;
    frame->length = length + WIFI_HEADER_PLUS_CHECKSUM;
    frame->frameControl = WIFI_FRAMECTL_PROBE_ASK;
    return frame;
}
