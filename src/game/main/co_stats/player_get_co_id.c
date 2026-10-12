#include "types.h"
#include "game/player.h"

int Player_GetCoByte(
    int player,
    int slot); /* CO byte of a player; slot 0 is the front CO, 1 the back CO */

int Player_GetCoId(int player, int slot) {
  return Player_GetCoByte(player, slot) & PLAYER_CO_ID_MASK;
}
