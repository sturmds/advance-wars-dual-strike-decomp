#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);
u32 WifiFrame_PutSsidElement(u8 *dest);
u32 WifiFrame_PutRatesElement(u8 *dest);
u32 WifiFrame_PutChannelElement(u8 *dest);
u32 WifiFrame_PutGameInfoElement(u8 *dest);

// Allocates a buffer and builds a probe response to dest: beacon interval,
// capability, then the SSID, rates, DS parameter and Nintendo elements. The
// caller fills the timestamp. Returns NULL when no buffer is free.
WlTxFrame *WifiFrame_NewProbeAnswer(const u16 *dest)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    WlTxBuffer *buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, link->gameInfoLength + 120);
    WlTxFrame *frame;
    u32 length;

    if (buffer == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
    frame = &buffer->frame;
    WifiTx_FillHeader(frame, dest);
    WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.beaconInterval = link->beaconInterval;
    WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.capability = link->flags_7c;
    length = WifiFrame_PutSsidElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.elements);
    length += WifiFrame_PutRatesElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.elements + length);
    length += WifiFrame_PutChannelElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.elements + length);
    /* 12: the timestamp, beacon interval and capability before the elements */
    frame->unk_06 = length + WifiFrame_PutGameInfoElement(WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.elements + length) + 12;
    frame->unk_12 = frame->unk_06 + WIFI_HEADER_PLUS_CHECKSUM;
    frame->frameControl = WIFI_FRAMECTL_PROBE_ANSWER;
    return frame;
}
