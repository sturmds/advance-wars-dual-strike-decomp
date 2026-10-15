#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"
#include "sdk/gx.h"

void GXS_SetGraphicsMode(int bgMode);

/* Sets the BG mode of the current screen and applies it to the main or sub
 * engine, depending on which screen is current. */
void Screen_SetBgMode(int bgMode) {
  GFX_BG_MODE(data_027c0280) = bgMode;
  if (data_027c027c == data_027c0288.unk_00) {
    GX_SetGraphicsMode(data_027c0280->dispMode, GFX_BG_MODE(data_027c0280),
                       data_027c0280->bg0Mode);
  } else {
    GXS_SetGraphicsMode(GFX_BG_MODE(data_027c0280));
  }
}
