#include "sdk/wl.h"

void func_arm7_027c80b0(int code);
void WifiTx_FillHeader(WlTxFrame *frame, const u16 *dest);

// Allocates a buffer and builds a deauthentication frame to dest with the
// given reason code. Returns NULL when no buffer is free.
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason)
{
    WlTxBuffer *buffer = (WlTxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 0x36);
    WlTxFrame *frame;

    if (buffer == NULL) {
        func_arm7_027c80b0(2);
        return (WlTxFrame *)buffer; /* needed to match: NULL, but returned from the register */
    }
    buffer->unk_0c = 0xffff;
    frame = &buffer->frame;
    WifiTx_FillHeader(frame, dest);
    frame->body = reason;
    frame->unk_06 = 2;
    frame->unk_12 = frame->unk_06 + 28;
    frame->frameControl = 0xc0;
    return frame;
}
