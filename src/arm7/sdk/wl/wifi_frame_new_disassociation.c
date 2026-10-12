#include "sdk/wl.h"

void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);

// Allocates a buffer and builds a disassociation frame to dest with the
// given reason code. Returns NULL when no buffer is free.
WlTxFrame *WifiFrame_NewDisassociation(const u16 *dest, u32 reason)
{
    WlTxBuffer *buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 0x36);
    WlTxFrame *frame;

    if (buffer == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
    frame = &buffer->frame;
    WifiTx_FillHeader(frame, dest);
    WIFI_MANAGEMENT_FIELDS_OF(frame)->reason = reason;
    frame->unk_06 = sizeof(WIFI_MANAGEMENT_FIELDS_OF(frame)->reason);
    frame->unk_12 = frame->unk_06 + WIFI_HEADER_PLUS_CHECKSUM;
    frame->frameControl = WIFI_FRAMECTL_DISASSOCIATION;
    return frame;
}
