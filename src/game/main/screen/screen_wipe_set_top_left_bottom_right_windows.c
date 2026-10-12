#include "types.h"
#include "game/screen_other_map.h"

/* Window 0 is the top left corner and window 1 the bottom right corner,
 * each width x height pixels of the 256x192 screen. A right edge of 0 stands
 * for the screen's right edge. */
void ScreenWipe_SetTopLeftBottomRightWindows(int width, int height) {
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = width;
  data_027c0280->win0Bottom = height;
  data_027c0280->win1Left = 256 - width;
  data_027c0280->win1Top = 192 - height;
  data_027c0280->win1Right = 0;
  data_027c0280->win1Bottom = 192;
}
