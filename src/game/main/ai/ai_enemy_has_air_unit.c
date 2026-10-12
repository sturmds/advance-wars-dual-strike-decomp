#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);

/* TRUE when a player on another team still in the game has an air unit.
 * Each scan skips the player's first unit slot and covers the next 50. */
BOOL Ai_EnemyHasAirUnit(void) {
  int player;
  int i;
  MapUnit* unit;

  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      continue;
    }
    for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
         i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
      unit = &STATE->units[i];
      if (unit->type != UNIT_NONE &&
          (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_AIR)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
