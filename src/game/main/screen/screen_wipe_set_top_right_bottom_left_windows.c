#include "types.h"
#include "game/screen_other_map.h"

/* Window 0 is the top right corner and window 1 the bottom left corner,
 * each width x height pixels of the 256x192 screen. A right edge of 0 stands
 * for the screen's right edge. */
void ScreenWipe_SetTopRightBottomLeftWindows(int width, int height) {
  data_027c0280->win0Left = 256 - width;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = 0;
  data_027c0280->win0Bottom = height;
  data_027c0280->win1Left = 0;
  data_027c0280->win1Top = 192 - height;
  data_027c0280->win1Right = width;
  data_027c0280->win1Bottom = 192;
}
