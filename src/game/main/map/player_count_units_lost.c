#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* needed to match: the original steps a byte pointer from the start of the
 * Player and adds the field offset as a constant. */
#define PLAYER_OFFSET(field) ((u32) & ((Player*)0)->field)

/* Units a player has lost, all types together. */
int Player_CountUnitsLost(int player) {
  int type;
  int total;
  u8* base;
  total = 0;
  base = (u8*)&STATE->players[player];

  for (type = 0; type < 27; type++) {
    total += base[PLAYER_OFFSET(unitsLost)];
    base++;
  }
  return total;
}
