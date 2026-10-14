#include "types.h"

extern u8 data_02151cc4[];
extern u8 data_02151cc8[];

/* Writes one 4-bit pixel of a linear 4bpp image, two pixels per byte. Keeps the other nibble
 * of the byte through the mask table and shifts value into place by the shift table. */
void Image4bpp_SetPixelLinear(u8* image, int x, int y, int width, u8 value) {
  u8* pair = &image[y * (width / 2) + x / 2];

  *pair =
      (*pair & data_02151cc4[(x & 1) ^ 1]) | (value << data_02151cc8[x & 1]);
}
