#include "types.h"
#include "game/ov003_unit.h"

void Ov003_DrawTexturedQuad(int x, int y, int a, int b, int c, int d, int e,
                            int f, int g);

/* Calls Ov003_DrawTexturedQuad for the point (x, y) shifted by (-9, -4) from the
 * camera, unless (x, y) is off screen. What it draws is not known. */
void Ov003_DrawIconAt(int x, int y) {
  if (x < data_ov003_02364928->scrollX - 22 ||
      x > data_ov003_02364928->scrollX + 265) {
    return;
  }
  if (y < data_ov003_02364928->scrollY - 11 ||
      y > data_ov003_02364928->scrollY + 196) {
    return;
  }
  Ov003_DrawTexturedQuad(x - data_ov003_02364928->scrollX - 9,
                         y - data_ov003_02364928->scrollY - 4, 23, 2, 1, 0x4c8,
                         17, 0, 0);
}
