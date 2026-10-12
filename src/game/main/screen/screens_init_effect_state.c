#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetBothNibbles(int value);
int func_02007710(int screens);

/* Sets up the screen effect state at start-up: both screens marked hidden,
 * effects aimed at the current screen, master brightness 0. */
void Screens_InitEffectState(void) {
  Screen_SetBothNibbles(1);
  func_02007710(0);
  data_027c0280->masterBrightness = 0;
}
