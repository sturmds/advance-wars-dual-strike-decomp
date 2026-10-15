#include "types.h"
#include "game/Unk_02088d04.h"
#include "sdk/gx.h"

/* Sets the display mode, BG mode and BG0 3D flag of the current screen, then
 * applies them to the main engine. */
void Screen_SetGraphicsMode(int dispMode, int bgMode, int bg0Is3d) {
  data_027c0280->dispMode = dispMode;
  GFX_BG_MODE(data_027c0280) =
      (u16)bgMode; /* needed to match: truncation done here, not at entry */
  data_027c0280->bg0Mode = bg0Is3d;
  GX_SetGraphicsMode(data_027c0280->dispMode, GFX_BG_MODE(data_027c0280),
                     data_027c0280->bg0Mode);
}
