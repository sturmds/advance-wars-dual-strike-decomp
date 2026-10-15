#include "types.h"
#include "game/screen_other_map.h"
#include "game/map_bg.h"

/* Draws map tile (mapX, mapY) into the BG tile maps. */
void MapView_DrawUnitTile(int mapX, int mapY);

#define VIEW (MAP_STATE->unk_1c4)

/* Redraws the 17 by 13 map tiles of the view, then runs the screen
 * config's unk_08 hook. */
void MapView_DrawAllTiles(void) {
  u16 col;
  u16 row;

  for (row = 0; row < MAP_VIEW_ROWS + 1; row++) {
    for (col = 0; col < MAP_VIEW_COLUMNS + 1; col++) {
      MapView_DrawUnitTile((s16)(col + (VIEW->scrollX >> 4)),
                           (s16)(row + (VIEW->scrollY >> 4)));
    }
  }
  if (data_027c027c->unk_50[data_027c0284]->unk_08 != NULL) {
    data_027c027c->unk_50[data_027c0284]->unk_08();
  }
}
