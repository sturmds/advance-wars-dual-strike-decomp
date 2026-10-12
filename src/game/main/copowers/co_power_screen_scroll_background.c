#include "types.h"
#include "game/screen_other_map.h"

/* Step of the CO Power screen script: scrolls background layer 2 sideways, 24 pixels a frame. */
void CoPowerScreen_ScrollBackground(void) {
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] += 24;
}
