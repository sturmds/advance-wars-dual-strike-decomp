#include "types.h"

/* Copies a block of tiles into a 32-tile-wide tile map at (x, y), adding
 * tileAdd to each entry and skipping cells outside the map. The block
 * starts with two bytes, width - 1 and height - 1, then the entries from
 * the bottom row up. The x check runs the column count down while dst
 * moves right, so partly hidden blocks clip on the wrong side. */
void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd) {
  u32 header = *(const u32*)block; /* needed to match: a word load */
  const u16* src = (const u16*)(block + 2);
  s16 lastCol = header & 0xff;
  s16 row = (header >> 8) & 0xff;
  u16* dst;
  s16 col;

  for (; row >= 0; row--) {
    if (y + row >= 0 && y + row < 32) {
      dst = map + (x + ((y + row) << 5));
      for (col = lastCol; col >= 0; col--, src++, dst++) {
        if (x + col >= 0 && x + col < 32) {
          *dst = *src + tileAdd;
        }
      }
    }
  }
}
