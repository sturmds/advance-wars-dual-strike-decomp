#include "types.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrentGfx(void* state);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Clears the 18x24 tile box at (14, 0) on layers 0 and 1 of the
 * Unk_027c0288.unk_04 graphics state, then restores the current one. */
void Ov000_ClearRightPanelBox(void) {
  Unk_027c0280* savedGfx;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Bg_FillRect(data_027c0280->layer0Map, 14, 0, 18, 24, 0);
  Bg_FillRect(data_027c0280->layer1Map, 14, 0, 18, 24, 0);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(savedGfx);
}
