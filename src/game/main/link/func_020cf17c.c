#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 8). Fails for a player who is out of the game, and for one who
 * is still in but has Player.unk_33 set. */
int func_020cf17c(u16 player) {
  if (!Player_IsInGame(player)) {
    return 0;
  }
  if (STATE->players[player].defeated == 0 &&
      STATE->players[player].unk_33 != 0) {
    return 0;
  }
  return 1;
}
