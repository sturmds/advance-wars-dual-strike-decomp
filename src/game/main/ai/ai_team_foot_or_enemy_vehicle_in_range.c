#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define MOVE_GROUP_GROUND \
  (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)

/* TRUE when a ground unit stands on a tile the range map reaches and is
 * either an Infantry or Mech of the current player's team, or an enemy
 * ground unit of any other type. Only slots 1-50 of each player count. */
BOOL Ai_TeamFootOrEnemyVehicleInRange(void) {
  int player;
  int slot;
  MapUnit* unit;

  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (!(Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_GROUND)) {
        continue;
      }
      if (STATE->rangeMap[unit->y][unit->x] == -1) {
        continue;
      }
      if (STATE->players[STATE->currentPlayer].team !=
          STATE->players[player].team) {
        if (unit->type <= UNIT_MECH) {
          continue;
        }
      } else {
        if (unit->type > UNIT_MECH) {
          continue;
        }
      }
      return TRUE;
    }
  }
  return FALSE;
}
