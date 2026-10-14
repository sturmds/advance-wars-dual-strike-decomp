#include "types.h"
#include "game/Unk_02088d04.h"

u8 Image4bpp_GetPixel(const u8* image, int x, int y);
void Image4bpp_SetPixelLinear(
    u8* image, int x, int y, int width,
    int value); /* writes a pixel of a row-major 4bpp image */

/* Converts a strip of `tiles` 8x8 tiles in tiled 4bpp layout (src) to a row-major
 * 4bpp image of width tiles * 8 and height 8 (dest). */
void Image4bpp_UntileStrip(const u8* src, u8* dest, int tiles) {
  int x;
  int y;
  int width = tiles * TILE_PIXELS;

  for (y = 0; y < TILE_PIXELS; y++) {
    for (x = 0; x < width; x++) {
      Image4bpp_SetPixelLinear(dest, x, y, width,
                               Image4bpp_GetPixel(src, x, y));
    }
  }
}
