#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_IsCurrentPlayers(int unitSlot);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Boards the acting unit on an own Black Boat or Lander it can reach that has
 * a free second cargo slot and can carry its type. A Black Boat beats a
 * Lander; later tiles win a tie. Marks the transport's AI target as its own
 * tile and moves the unit onto it. */
void AiUnit_BoardSeaTransport(void) {
  u8 type;
  int bestX;
  MapUnit* unit;
  int bestY;
  u8 bestType;
  int x;
  int y;
  bestX = -1;
  bestY = -1;
  bestType = 0;

  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->mapTiles->unitAt[TILE(x, y)] == 0 ||
          !Unit_IsCurrentPlayers(STATE->mapTiles->unk_508[TILE(x, y)])) {
        continue;
      }
      unit = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
      type = unit->type;
      if (type != UNIT_BLACK_BOAT && type != UNIT_LANDER) {
        continue;
      }
      if ((Ov000_gUnitStats[type].carryTypes + STATE->actingUnit->type)[1] ==
          0) {
        continue;
      }
      if (unit->cargo[1] != 0) {
        continue;
      }
      switch (bestType) {
        case UNIT_NONE:
        case UNIT_LANDER:
          break;
        case UNIT_BLACK_BOAT:
          if (type == UNIT_LANDER) {
            continue;
          }
          break;
        default:
          continue;
      }
      bestX = x;
      bestY = y;
      bestType = unit->type; /* needed to match: not type */
    }
  }
  if (bestX == -1) {
    return;
  }
  STATE->actingUnit->transportWanted = 0;
  if (STATE->actingUnit->aiOrder == 2 || STATE->actingUnit->aiOrder == 6) {
    STATE->actingUnit->aiOrder = 0;
    STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
    STATE->actingUnit->aiTargetY = MAP_UNIT_NO_TARGET;
  }
  unit = &STATE->units[STATE->mapTiles->unitAt[TILE(bestX, bestY)]];
  unit->aiTargetX = bestX;
  unit->aiTargetY = bestY;
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_BOARD, 0, 0);
}
