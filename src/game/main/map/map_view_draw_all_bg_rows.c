#include "types.h"
#include "game/screen_other_map.h"
#include "game/map_bg.h"

void MapBg_DrawTileRow(u16 startCol, u16 row, s16 tileX, s16 tileY);

#define VIEW (MAP_STATE->unk_1c4)
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Redraws the 13 rows of the background buffer from the map view's current
 * scroll, then runs the screen config's unk_10 and unk_0c hooks. */
void MapView_DrawAllBgRows(void) {
  s16 i;

  for (i = 0; i < MAP_VIEW_ROWS + 1; i++) {
    MapBg_DrawTileRow((VIEW->scrollX >> 4) - VIEW->unk_26,
                      i + ((VIEW->scrollY >> 4) - VIEW->unk_28),
                      VIEW->scrollX >> 4, i + (VIEW->scrollY >> 4));
  }
  if (HOOKS->unk_10 != NULL) {
    HOOKS->unk_10();
  }
  if (HOOKS->unk_0c != NULL) {
    HOOKS->unk_0c();
  }
}
