#include "types.h"
#include "game/map_bg.h"

void MapView_DrawUnitTile(int x, int y);

/* Draws the units of one row of 17 tiles at mapY, starting at column mapX. */
void MapView_DrawUnitRow(int bufX, int bufY, int mapX, int mapY) {
  u16 i;

  for (i = 0; i < MAP_VIEW_COLUMNS + 1; i++) {
    MapView_DrawUnitTile((s16)(mapX + i), mapY);
  }
}
