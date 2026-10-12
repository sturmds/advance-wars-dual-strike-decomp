#include "types.h"

void ResultPath_DrawSprite(int step, int sprite);

/* Draws the four path sprites, each 35 steps behind the one before it.
 * `lead` is the step of sprite 3. */
void ResultPath_DrawAll(int lead) {
  int i;
  int behind;

  behind = i = 0;
  for (; i < 4; i++) {
    ResultPath_DrawSprite(lead - behind, 3 - i);
    behind += 35;
  }
}
