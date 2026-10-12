#include "types.h"

void ResultPath_DrawSpriteOffset(int step, int sprite, int dx, int dy);

/* ResultPath_DrawAll shifted by (dx, dy) pixels. */
void ResultPath_DrawAllOffset(int lead, int dx, int dy) {
  int i;
  int behind;

  behind = i = 0;
  for (; i < 4; i++) {
    ResultPath_DrawSpriteOffset(lead - behind, 3 - i, dx, dy);
    behind += 35;
  }
}
