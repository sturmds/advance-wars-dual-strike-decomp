#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_bg.h"

void MapView_DrawRangeTile(s16 x, s16 y);

/* Draws the range overlay for one row of 17 tiles at mapY, starting at column
 * mapX. */
void MapView_DrawRangeRow(int bufX, int bufY, s16 mapX, s16 mapY) {
  u16 i;

  if (MAP_STATE->unk_1c4->mapHeight - mapY > 12) {
    for (i = 0; i < MAP_VIEW_COLUMNS + 1; i++) {
      MapView_DrawRangeTile(mapX + i, mapY);
    }
  }
}
