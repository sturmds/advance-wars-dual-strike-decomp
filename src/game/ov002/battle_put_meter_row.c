#include "types.h"

void Bg_PutTileRect(u16* map, u16 x, u16 y, int width, u16 height,
                    int firstTile, u16 palette);

/* Puts one row of 4 tiles of a meter in the battle screen. Its only caller
 * draws the HP bar; the argument names are guesses from that call. */
void Battle_PutMeterRow(u16* map, u8 x, u8 y, u16 tile, u8 palette, u8 level,
                        u8 fullStep, u8 levelEnd, u8 levelSplit) {
  if (level < levelEnd) {
    if (level <= levelSplit) {
      tile = tile + (fullStep - 1) * 4;
    } else {
      tile = tile + (levelEnd - level) * 4;
    }
  }
  Bg_PutTileRect(map, x, y, 4, 1, tile, palette);
}
