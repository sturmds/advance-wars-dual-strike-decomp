#include "types.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the nonzero tiles in the property list (Unk_02080440_48.unk_608c) of
 * the screen in data_027c0288.unk_00, up to the 0xff entry. The declaration
 * order is needed to match. */
int Ov006_CountPropertyTiles(void) {
  int count;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  saved0280 = data_027c0280;
  count = 0;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 92; i++) {
    if (STATE->unk_608c[i].tile == 0xff) {
      break;
    }
    if (STATE->unk_608c[i].tile != 0) {
      count++;
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return count;
}
