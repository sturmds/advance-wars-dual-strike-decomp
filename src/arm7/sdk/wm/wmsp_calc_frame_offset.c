#include "sdk/gx.h"
#include "sdk/wmsp.h"

/* A frame lasts 263 lines of 63.5 us: 16715 us. */
#define US_PER_FRAME 0x414b
/* Lines are 63.5 us long, so the sum below counts half microseconds. */
#define HALF_US_PER_LINE 127
#define US_COUNT_MASK 0x3fffc0

/*
 * WMStatus.unk_cc holds WMSP_FRAME_START_TIME (64 us units). Turns it into
 * microseconds, steps it one frame at a time until it passes the start of the next
 * frame, and stores in unk_d4 how far past it ends up. unk_d4 is 0 when the time is
 * already past that start, needs 30 steps or more, or ends up more than 0x400e us past it.
 */
void WMSP_CalcFrameOffset(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    u32 lo;
    u32 hi;
    u32 now;
    int frame;
    u32 nextFrame;
    u32 vcount;

    buf->status->unk_cc <<= 6;

    /* Read the 32-bit counter in halves; read the high half again if the low half wrapped. */
    lo = W_US_COUNT0;
    hi = W_US_COUNT1;
    now = W_US_COUNT0;
    if (lo > now) {
        hi = W_US_COUNT1;
    }
    now = hi << 16 | now;
    vcount = *REG_VCOUNT;
    now &= US_COUNT_MASK;
    nextFrame = ((263 - vcount) * HALF_US_PER_LINE + (now << 1)) >> 1 & US_COUNT_MASK;

    if (buf->status->unk_cc > nextFrame) {
        buf->status->unk_d4 = 0;
        return;
    }
    for (frame = 1; frame < 30; frame++) {
        buf->status->unk_cc += US_PER_FRAME;
        if (buf->status->unk_cc > nextFrame) {
            buf->status->unk_d4 = buf->status->unk_cc - nextFrame;
            if (buf->status->unk_d4 > 0x400e) {
                buf->status->unk_d4 = 0;
            }
            return;
        }
    }
    buf->status->unk_d4 = 0;
}
