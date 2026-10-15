#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0216e2b0.h"

/* Window 0 covers the whole screen (256x192); window 1 is set to its off values.
 * The first store to win0Right is needed to match. */
void Screen_ResetWindowRects(void) {
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = 0;
  data_027c0280->win0Bottom = 192;
  data_027c0280->win0Right = 255;
  data_027c0280->win1Left = 255;
  data_027c0280->win1Top = 0;
  data_027c0280->win1Right = 1;
  data_027c0280->win1Bottom = 192;
  data_0216e2b0.unk_02 = 0;
  data_0216e2b0.unk_03 = 192;
}
