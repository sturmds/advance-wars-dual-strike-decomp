#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void GX_SetGraphicsMode(int displayMode, int bgMode, int bg0Is3d);

/* Sets whether BG0 shows the 3D engine and applies the graphics mode (main engine only). */
void Screen_SetBg0Mode(int bg0Is3d) {
  data_027c0280->bg0Mode = bg0Is3d;
  GX_SetGraphicsMode(data_027c0280->dispMode, GFX_BG_MODE(data_027c0280),
                     data_027c0280->bg0Mode);
}
