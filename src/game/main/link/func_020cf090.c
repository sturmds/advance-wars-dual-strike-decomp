#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Player_HasUnitsLeft(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* One of four per-player checks run by LinkPlayers_RecordFirstFailure (this one
 * records code 1). Passes unless bit 1 of unk_1c4->unk_37 is set; then
 * Player_HasUnitsLeft decides. */
int func_020cf090(u16 player) {
  if (!(STATE->unk_1c4->unk_37 & 1)) {
    return 1;
  }
  return Player_HasUnitsLeft(player);
}
