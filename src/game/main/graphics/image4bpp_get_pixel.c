#include "types.h"

extern u8 gNibbleMasks[2]; /* indexed by x & 1 */
extern u8 gNibbleShifts[2];
extern u8 gTile4bppPixelOffsets
    [64]; /* byte offset of each pixel inside an 8x8 tile */

/* Reads the 4bpp pixel at (x, y) from an image stored as 8x8 tiles of
 * 32 bytes, tiles in a row left to right. */
u8 Image4bpp_GetPixel(const u8* image, int x, int y) {
  int pixel = x % 8 + y % 8 * 8;
  int tile = x / 8;
  int odd = pixel & 1;
  int offset = gTile4bppPixelOffsets[pixel] + tile * 32;
  int pair = image[offset];

  return (pair & gNibbleMasks[odd]) >> gNibbleShifts[odd];
}
