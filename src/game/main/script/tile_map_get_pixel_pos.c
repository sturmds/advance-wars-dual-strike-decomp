#include "types.h"

s32 HwDiv32(s32 numerator, s32 denominator);

/* Pixel position (x, y) of a tile in a 32-tile-wide tile map of 8x8-pixel
 * tiles. */
void TileMap_GetPixelPos(u16* tile, u16* tileMap, s16* pixelPos) {
  int offset = (u8*)tile - (u8*)tileMap;

  pixelPos[0] = ((u32)(offset & 0x3f) >> 1)
                << 3; /* unsigned shift needed to match */
  pixelPos[1] = HwDiv32(offset, 0x40) << 3;
}
