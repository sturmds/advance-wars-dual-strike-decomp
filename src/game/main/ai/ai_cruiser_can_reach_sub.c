#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
void RangeMap_MarkAdjacentTiles(int arg);
s16 AiActingUnit_GetSearchMovement(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the acting unit is a Cruiser and it can reach or get next to an
 * enemy Sub this turn. The range map is pointed at MapTiles.rangeScratch while
 * it is filled, then back at MapTiles.rangeMapTiles. */
int AiUnit_CruiserCanReachSub(void) {
  int player;
  int i;
  MapUnit* acting;

  if (STATE->actingUnit->type != UNIT_CRUISER) {
    return 0;
  }
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  acting = STATE->actingUnit;
  RangeMap_FillMoves(acting->x, acting->y, acting->type,
                     AiActingUnit_GetSearchMovement(), 1);
  RangeMap_MarkAdjacentTiles(111);
  for (player = 1; player < 5; player++) {
    if (Player_IsInGame(player) && STATE->players[STATE->currentPlayer].team !=
                                       STATE->players[player].team) {
      for (i = Ov000_gPlayerFirstUnitSlot[player] + 1;
           i < Ov000_gPlayerFirstUnitSlot[player] + 51; i++) {
        MapUnit* unit = &STATE->units[i];

        if (unit->type != UNIT_NONE && unit->type == UNIT_SUB &&
            STATE->rangeMap[unit->y][unit->x] >= 0) {
          RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
          return 1;
        }
      }
    }
  }
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  return 0;
}
