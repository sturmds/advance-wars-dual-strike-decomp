#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Popup row for the cursor: its screen row in 8-pixel tiles (map tiles are
 * two high), one less from row 6 and again from row 19, and at least 2. */
int UnitMenu_GetPopupY(void) {
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];
  int y = (game->cursorY - game->unk_1c4->scrollY / 16) * 2;

  if (y >= 6) {
    y--;
  }
  if (y >= 19) {
    y--;
  }
  if (y < 2) {
    y = 2;
  }
  return y;
}
