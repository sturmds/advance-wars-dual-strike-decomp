#include "types.h"

extern u8 data_02151cc4[2];
extern u8 data_02151cc8[2];

void Image4bpp_SetPixelLinear(u8 *image, int x, int y, int width, u8 value)
{
    int offset = y * (width / 2) + x / 2;
    int odd = x & 1;
    int pair = image[offset];

    image[offset] = (pair & data_02151cc4[odd ^ 1]) | (value << data_02151cc8[odd]);
}
