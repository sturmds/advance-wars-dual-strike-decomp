#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_item.h"

void Ov003_DrawTexturedQuad(int x, int y, int a2, int a3, int a4, int a5,
                            int a6, int a7, int a8);

/* Draws one item at its pixel position, unless it is off screen. */
void Ov003_DrawItem(Ov003Item* item) {
  /* needed to match: s16 locals */
  s16 pixelX;
  s16 pixelY;

  pixelX = item->x >> 3;
  if (pixelX < data_ov003_02364928->scrollX - 7 ||
      pixelX > data_ov003_02364928->scrollX + 264) {
    return;
  }
  pixelY = item->y >> 3;
  if (pixelY < data_ov003_02364928->scrollY - 7 ||
      pixelY > data_ov003_02364928->scrollY + 200) {
    return;
  }
  Ov003_DrawTexturedQuad(pixelX - data_ov003_02364928->scrollX - 8,
                         pixelY - data_ov003_02364928->scrollY - 8, 15, 1, 1,
                         0x4d1 + (item->kind - 1) * 4, 18, 0, 0);
}
