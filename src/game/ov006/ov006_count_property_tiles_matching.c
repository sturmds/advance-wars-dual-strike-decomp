#include "types.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Like Ov006_CountPropertyTiles, but counts only the entries whose tile byte
 * (terrain and owner) equals tile. The declaration order is needed to match. */
int Ov006_CountPropertyTilesMatching(int tile) {
  int i;
  int count;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  count = 0;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 92; i++) {
    if (STATE->unk_608c[i].tile == 0xff) {
      break;
    }
    if (STATE->unk_608c[i].tile != 0 && tile == STATE->unk_608c[i].tile) {
      count++;
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return count;
}
