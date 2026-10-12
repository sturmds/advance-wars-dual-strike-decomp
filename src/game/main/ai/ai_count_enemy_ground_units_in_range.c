#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the ground units of every player in the game on another team than
 * the current player that stand on a tile the range map reaches. Units being
 * carried are skipped. Only slots 1-50 of each player count. */
int Ai_CountEnemyGroundUnitsInRange(void) {
  int count = 0;
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &STATE->units[slot];
      if (unit->type == UNIT_NONE || (unit->flags & MAP_UNIT_CARRIED)) {
        continue;
      }
      if (!(Ov000_gUnitStats[unit->type].moveGroup &
            (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
        continue;
      }
      if (STATE->rangeMap[unit->y][unit->x] != -1) {
        count++;
      }
    }
  }
  return count;
}
