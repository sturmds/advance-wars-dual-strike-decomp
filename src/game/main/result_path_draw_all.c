#include "types.h"
#include "game/result_sprite_task.h"

void ResultPath_DrawSprite(int step, int sprite);

/* Draws the RESULT_PATH_SPRITE_COUNT path sprites, each RESULT_PATH_SPACING
 * steps behind the one before it.
 * `lead` is the step of the last sprite. */
void ResultPath_DrawAll(int lead) {
  int i;
  int behind;

  behind = i = 0;
  for (; i < RESULT_PATH_SPRITE_COUNT; i++) {
    ResultPath_DrawSprite(lead - behind, RESULT_PATH_SPRITE_COUNT - 1 - i);
    behind += RESULT_PATH_SPACING;
  }
}
