#include "sdk/wl.h"

void WL_WriteByte(void *addr, u32 value);
void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);

// Allocates a buffer and builds an authentication frame to dest. A nonzero
// challengeLength adds a challenge text element of that length; the caller
// fills the body fields and the challenge bytes. Returns NULL when no buffer
// is free.
WlTxFrame *WifiFrame_NewAuthentication(const u16 *dest, u16 challengeLength)
{
    WlTxBuffer *buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, challengeLength + 0x13a);
    WlTxFrame *frame;

    if (buffer == NULL) {
        WifiHost_RaiseEventBits(WIFI_EVT_TX_BUILD_FAILED);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = WIFI_DRIVER_BUILT_FRAME;
    frame = &buffer->frame;
    WifiTx_FillHeader(frame, dest);
    if (challengeLength != 0) {
        WL_WriteByte(&WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.elements[0], WIFI_CHALLENGE_IE_ID);
        WL_WriteByte(&WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.elements[1], (u8)challengeLength);
        challengeLength += 2;
    }
    frame->bodyLength = challengeLength + 6;
    frame->length = frame->bodyLength + WIFI_HEADER_PLUS_CHECKSUM;
    frame->frameControl = WIFI_FRAMECTL_AUTHENTICATION;
    return frame;
}
