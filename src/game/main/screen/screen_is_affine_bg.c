#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* TRUE when the BG (0-3) is a rotation/scaling BG in the current BG mode:
 * BG3 in mode 1, BG2 and BG3 in mode 2, BG2 in mode 4. */
int Screen_IsAffineBg(int bg) {
  switch (GFX_BG_MODE(data_027c0280)) {
    case 1:
      if (bg == 3) {
        return 1;
      }
      return 0;
    case 2:
      if (bg - 2 <= 1u) {
        return 1;
      }
      return 0;
    case 4:
      if (bg == 2) {
        return 1;
      }
      return 0;
    default:
      return 0;
  }
}
