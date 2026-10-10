#include "types.h"
#include "game/Unk_ov005_0235b574.h"

/* 87%: same instructions, address set-up scheduled differently. Copies 8
 * rows of 4 bytes of two planes from buffer_0010 to buffer_2010. */
void func_ov005_0235b4c0(Unk_ov005_0235b574 *work, int x, int y)
{
    int dstOffset;
    int srcOffset;
    u8 *s;
    int col;
    u8 *d;
    dstOffset = ((x & 3) << 5) + ((x & 0xfffc) << 6);
    srcOffset = ((y & 31) << 5) + ((y & 0xffe0) << 6);
    d = work->buffer_2010 + dstOffset;
    s = work->buffer_0010 + srcOffset;

    for (col = 0; col < 32; col += 4) {
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
        d[3] = s[3];
        d[0x80] = s[0x400];
        d[0x81] = s[0x401];
        d[0x82] = s[0x402];
        d[0x83] = s[0x403];
        s += 4;
        d += 4;
    }
}
