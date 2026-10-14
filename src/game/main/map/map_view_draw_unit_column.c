#include "types.h"
#include "game/map_bg.h"

void MapView_DrawUnitTile(int x, int y);

/* Draws the units of one column of 13 tiles at mapX, starting at row mapY. */
void MapView_DrawUnitColumn(int bufX, int bufY, int mapX, int mapY) {
  u16 i;

  for (i = 0; i < MAP_VIEW_ROWS + 1; i++) {
    MapView_DrawUnitTile(mapX, (s16)(mapY + i));
  }
}
