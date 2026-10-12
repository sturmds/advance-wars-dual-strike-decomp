#include "types.h"
#include "game/map_data.h"

/* Fills a 32-wide screen tile map for the map preview with consecutive tile numbers from
 * firstTile: one entry per two map columns, one row per two map rows. y steps by 16 per
 * row, so y + y is the row's first entry (needed to match: not y * 2 or a row variable). */
void MapPreview_FillTileMap(u16* screen, int firstTile) {
  int x;
  int y;
  int tile;
  int width;
  int height;

  tile = 0;
  width = (data_0229eadc->width + 1) & ~1;
  height = (data_0229eadc->height + 1) & ~1;
  for (y = 0; y < (height >> 1) << 4; y += 16) {
    for (x = 0; x < width >> 1; x++) {
      screen[(y + y) + x] = firstTile + tile;
      tile++;
    }
  }
}
