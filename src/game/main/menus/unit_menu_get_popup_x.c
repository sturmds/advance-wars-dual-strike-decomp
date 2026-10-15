#include "types.h"
#include "game/Unk_02080440.h"

/* Screen column of the map cursor in 8-pixel tiles (map tiles are two wide). */
int UnitMenu_GetPopupX(void) {
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];

  return (game->cursorX - game->unk_1c4->scrollX / 16) * 2;
}
