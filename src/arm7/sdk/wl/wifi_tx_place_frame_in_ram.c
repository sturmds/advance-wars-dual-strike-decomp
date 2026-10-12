#include "sdk/wl.h"

void WifiTx_CopyHeaderAndBody(void *dest, const void *header, const void *body, u32 bodySize);
void WifiTx_CopyWithWepGap(void *dest, const void *header, const void *body, u32 bodySize);
void func_arm7_027c84e0(void *dest, const void *src, u32 size);

// Copies a queued frame into WIFI_PACKET_RAM for sending. Frames the driver built
// (WIFI_DRIVER_BUILT_FRAME) hold their body inline; host frames hold a pointer to it
// there. A protected frame gets a WEP header: a random IV, the key index in
// the top bits.
void WifiTx_PlaceFrameInRam(WlRamFrame *dest, WlTxBuffer *buffer)
{
    WlTxFrame *frame = &buffer->frame;

    if (frame->frameControl & WIFI_FRAMECTL_WEP_ON) {
        if (buffer->unk_0c == WIFI_DRIVER_BUILT_FRAME) {
            WifiTx_CopyWithWepGap(dest, &frame->result, &frame->body, frame->unk_06);
        } else {
            WifiTx_CopyWithWepGap(dest, &frame->result, *(void **)&frame->body, frame->unk_06);
        }
        dest->body[0] = REG_W_RANDOM + (REG_W_RANDOM << 8);
        dest->body[1] = (REG_W_RANDOM & 0xff) | (WIFI_DRIVER_STATE->state.unk_0a << 14);
        if (WIFI_DRIVER_STATE->unk_638 & 8) { /* clear the 4 bytes before the FCS */
            u16 *icv = (u16 *)(((u32)&dest->frameControl + frame->unk_12 - 7) & ~1);
            icv[0] = 0;
            icv[1] = 0;
        }
    } else if (buffer->unk_0c == WIFI_DRIVER_BUILT_FRAME) {
        func_arm7_027c84e0(dest, &frame->result, frame->unk_06 + 0x24);
    } else {
        WifiTx_CopyHeaderAndBody(dest, &frame->result, *(void **)&frame->body, frame->unk_06);
    }
    if (WIFI_DRIVER_STATE->unk_638 & 4) {
        u16 *footer = (u16 *)(((u32)&dest->frameControl + frame->unk_12 - 1) & ~3);
        footer[0] = WIFI_GUARD_WORD_A;
        footer[1] = WIFI_GUARD_WORD_B;
    }
}
