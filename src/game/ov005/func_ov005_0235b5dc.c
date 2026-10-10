#include "types.h"
#include "game/Unk_ov005_0235b574.h"

/* 15%: copies 4-byte pieces of two planes (0x400 apart) from buffer_0010 to
 * buffer_6010 in 8 rows; scheduling differs. */
void func_ov005_0235b5dc(Unk_ov005_0235b574 *work, int x, int y)
{
    int srcOffset = ((y & 31) << 5) + ((y & 0xffe0) << 6);
    int dstOffset = ((x & 31) << 2) + ((x & 0xffe0) << 6);
    u8 *dst = work->buffer_6010 + dstOffset;
    int col = 0;
    int row = 0;

    for (; col < 32; col += 4, row++) {
        u8 *s = work->buffer_0010 + col + srcOffset;
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
