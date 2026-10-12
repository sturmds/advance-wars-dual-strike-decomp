#include "types.h"
#include "game/ov003_unit.h"

void Ov003_DrawTexturedQuad(int x, int y, int a, int b, int c, int d, int e,
                            int f, int g);

/* Like Ov003_DrawIconAt, but the shift is (-1, -4) and frame picks the fifth
 * argument of Ov003_DrawTexturedQuad from a table of 4-byte steps at 0x47c. */
void Ov003_DrawFrameAt(int x, int y, int frame) {
  if (x < data_ov003_02364928->scrollX - 14 ||
      x > data_ov003_02364928->scrollX + 257) {
    return;
  }
  if (y < data_ov003_02364928->scrollY - 11 ||
      y > data_ov003_02364928->scrollY + 196) {
    return;
  }
  Ov003_DrawTexturedQuad(x - data_ov003_02364928->scrollX - 1,
                         y - data_ov003_02364928->scrollY - 4, 23, 1, 1,
                         0x47c + (frame - 1) * 4, 17, 0, 0);
}
