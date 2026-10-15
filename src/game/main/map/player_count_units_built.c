#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* needed to match: the original steps a byte pointer from the start of the
 * Player and adds the field offset as a constant. */
#define PLAYER_OFFSET(field) ((u32) & ((Player*)0)->field)

/* Units a player has built, all types together. */
int Player_CountUnitsBuilt(int player) {
  int type;
  int total;
  u8* base;
  total = 0;
  base = (u8*)&MAP_STATE->players[player];

  for (type = 0; type < UNIT_TYPE_COUNT; type++) {
    total += base[PLAYER_OFFSET(unitsBuilt)];
    base++;
  }
  return total;
}
