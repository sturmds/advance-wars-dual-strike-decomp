#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* TRUE when the BG numbered like this layer (0-3) is a text (tiled) BG in the
 * current BG mode: the BGs below the first rotation/scaling one. */
BOOL Screen_IsTextBg(u32 layer) {
  switch (GFX_BG_MODE(data_027c0280)) {
    case 0:
      return TRUE;
    case 1:
    case 3:
      return layer <= 2;
    case 2:
    case 4:
    case 5:
      return layer <= 1;
    default:
      return FALSE;
  }
}
