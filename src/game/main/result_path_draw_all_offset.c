#include "types.h"
#include "game/result_sprite_task.h"

void ResultPath_DrawSpriteOffset(int step, int sprite, int dx, int dy);

/* ResultPath_DrawAll shifted by (dx, dy) pixels. */
void ResultPath_DrawAllOffset(int lead, int dx, int dy) {
  int i;
  int behind;

  behind = i = 0;
  for (; i < RESULT_PATH_SPRITE_COUNT; i++) {
    ResultPath_DrawSpriteOffset(lead - behind, RESULT_PATH_SPRITE_COUNT - 1 - i,
                                dx, dy);
    behind += RESULT_PATH_SPACING;
  }
}
