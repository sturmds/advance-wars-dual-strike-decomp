#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 8). Fails for a player who is out of the game, and for one who
 * is still in but has Player.unk_33 set. */
int func_020cf17c(u16 player) {
  if (!Player_IsInGame(player)) {
    return 0;
  }
  if (MAP_STATE->players[player].defeated == 0 &&
      MAP_STATE->players[player].unk_33 != 0) {
    return 0;
  }
  return 1;
}
