#include "types.h"

/* Copies a width x height block of tilemap entries; rows are 32 entries apart. */
void func_ov002_0235d4a8(const u16 *src, u16 *dst, int width, int height)
{
    int col;
    int row;
    int offset;
    row = 0;
    offset = 0;
    for (; row < height; row++) {
        for (col = 0; col < width; col++) {
            *(dst + offset + col) = *(src + offset + col);
        }
        offset += 32;
    }
}
