#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 2). Passes unless bit 2 of unk_1c4->unk_37 is set; then the
 * player must be in the game and have Player.unk_34 clear. */
int func_020cf0e4(u16 player) {
  Player* p;

  if (!(MAP_STATE->unk_1c4->unk_37 & 2)) {
    return 1;
  }
  if (!Player_IsInGame(player)) {
    return 0;
  }
  p = &MAP_STATE->players[player];
  if (p->unk_34 == 0) {
    return 1;
  }
  return 0;
}
