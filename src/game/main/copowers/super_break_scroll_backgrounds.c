#include "types.h"
#include "game/screen_other_map.h"

/* Step of the Super CO Power cut-in script (SuperBreak_SetUpScreen's): scrolls the
 * background layers 2 and 1 sideways, 24 and 12 pixels a frame, for a parallax look. */
void SuperBreak_ScrollBackgrounds(void) {
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] += 24;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] += 12;
}
