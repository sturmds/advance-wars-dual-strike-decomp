#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* CO byte of a player's CO slot (0 front, 1 back), read from the right
 * screen's game state. In a two-map battle where the other map's unk_30 is 0,
 * slot 0 reads the first screen and slot 1 reads the second, each its front CO. */
int Player_GetCoByte(int player, int slot) {
  Unk_02080440* saved027c = data_027c027c;
  Unk_027c0280* saved0280;
  int coByte;

  saved0280 = data_027c0280;
  if (GAME->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
    if (slot == 0) {
      Screen_SetCurrentBoth(data_027c0288.unk_00);
    } else {
      Screen_SetCurrentBoth(data_027c0288.unk_04);
    }
    coByte = GAME->players[player].front.id;
  } else {
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    coByte = (&GAME->players[player].front)[slot].id;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return coByte;
}
