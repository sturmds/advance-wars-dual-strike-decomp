#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Mem_Copy32(const void* src, void* dst, u32 size);
void Bg_ClearRect(void* target, int x, int y, int w, int h);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Called by the DayBanner functions after they set the scroll: copies the saved banner map from gMenuState back to
 * layer 0, then clears the tile rows above the scroll position and the rows below
 * the 192-pixel screen height, so only the visible part of the banner is left.
 * Needed to match: one variable holds the top edge, then the bottom edge. */
void DayBanner_RefreshLayer(void) {
  int edge;
  int row;

  Mem_Copy32(&gMenuState, data_027c0280->layer0Map, BG_SCREEN_SIZE);
  edge = (s16)data_027c0280->bgScrollY[data_027c0280->layer0Bg];
  if (edge < 0) {
    edge = 0;
  }
  Bg_ClearRect(data_027c0280->layer0Map, 0, 0, 32, (u32)(edge << 13) >> 16);
  edge = (s16)(((s16)data_027c0280->bgScrollY[data_027c0280->layer0Bg] + 199) &
               ~7);
  if (edge > 256) {
    edge = 256;
  }
  row = (edge << 13) >> 16;
  Bg_ClearRect(data_027c0280->layer0Map, 0, (u16)row, 32, (u16)(32 - row));
  Gfx_RequestLayerCopy(0);
}
