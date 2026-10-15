#include "types.h"
#include "game/screen_other_map.h"
#include "game/map_bg.h"

void MapBg_DrawMaskRow(u16 startCol, u16 row, s16 tileX, s16 tileY);

#define VIEW (MAP_STATE->unk_1c4)
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Like MapView_DrawAllBgRows with MapBg_DrawMaskRow drawing each row, and only
 * the unk_0c hook afterwards. MapBg_DrawMaskRow writes only layer 3, hence
 * the name. */
void MapView_DrawAllBgRowsGroundOnly(void) {
  s16 i;

  for (i = 0; i < MAP_VIEW_ROWS + 1; i++) {
    MapBg_DrawMaskRow((VIEW->scrollX >> 4) - VIEW->unk_26,
                      i + ((VIEW->scrollY >> 4) - VIEW->unk_28),
                      VIEW->scrollX >> 4, i + (VIEW->scrollY >> 4));
  }
  if (HOOKS->unk_0c != NULL) {
    HOOKS->unk_0c();
  }
}
