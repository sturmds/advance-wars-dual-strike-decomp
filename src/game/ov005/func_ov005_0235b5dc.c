#include "types.h"
#include "game/Unk_ov005_0235b574.h"

/* 72%: the original masks x and y, then shifts each part as its own instruction; ours folds
 * the shifts into the adds. */
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off
#pragma opt_dead_assignments off

/* Copies 8 rows of 4 bytes, in each of two planes 0x400 bytes apart, from buffer_0010 at
 * cell y to buffer_6010 at cell x. */
void func_ov005_0235b5dc(Unk_ov005_0235b574 *work, int x, int y)
{
    int xLo = x & 31;
    int yLo = y & 31;
    int xHi = x & 0xffe0;
    int yHi = y & 0xffe0;
    int col = 0;
    int row = 0;
    u8 *src = work->buffer_0010;
    u8 *dst = work->buffer_6010;
    int srcOffset;

    xLo <<= 2;
    yLo <<= 5;
    xHi <<= 6;
    yHi <<= 6;
    dst += xLo + xHi;
    srcOffset = yLo + yHi;
    for (; col < 32; col += 4, row++) {
        u8 *s = src + col + srcOffset;
        u8 *d = dst + (row << 7);

        d[0] = s[0];
        d[0x400] = s[0x400];
        d[1] = s[1];
        d[0x401] = s[0x401];
        d[2] = s[2];
        d[0x402] = s[0x402];
        d[3] = s[3];
        d[0x403] = s[0x403];
    }
}
