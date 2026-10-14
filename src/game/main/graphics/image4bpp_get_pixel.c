#include "types.h"

u8 data_02151cc4[] = {
#include "data/main/data_02151cc4.inc.c"
};
static u8 gNibbleShifts[] = {
#include "data/main/gNibbleShifts.inc.c"
};
/* indexed by x & 1 */
static u8 gNibbleMasks[] = {
#include "data/main/gNibbleMasks.inc.c"
};
u8 data_02151cc8[] = {
#include "data/main/data_02151cc8.inc.c"
};
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
