#include "sdk/mi.h"

void MI_UncompressLZ8(const u8 *src, u16 *dest)
{
    int remaining;
    u32 pending;
    u32 shift;
    u32 flags;
    int length;
    int disp;
    int lane;
    int bits;

    pending = 0;
    shift = 0;
    remaining = *(const u32 *)src >> 8;
    src += 4;
    for (;;) {
        if (remaining <= 0) {
            return;
        }
        flags = *src++;
        bits = 8;
        for (;;) {
            if (--bits < 0) {
                break;
            }
            if (!(flags & 0x80)) {
                pending |= *src++ << shift;
                remaining--;
                shift ^= 8;
                if (shift == 0) {
                    *dest++ = pending;
                    pending = 0;
                }
            } else {
                length = 3 + (*src >> 4);
                disp = *src++ & 0xf;
                disp = ((disp << 8) | *src++) + 1;
                remaining -= length;
                lane = (8 - shift) ^ ((disp & 1) << 3);
                do {
                    lane ^= 8;
                    pending |= ((*(u16 *)((u8 *)dest - (((disp + ((8 - shift) >> 3)) >> 1) << 1)) & (0xff << lane)) >> lane) << shift;
                    shift ^= 8;
                    if (shift == 0) {
                        *dest++ = pending;
                        pending = 0;
                    }
                } while (--length > 0);
            }
            if (remaining > 0) {
                flags <<= 1;
            } else {
                break;
            }
        }
    }
}
