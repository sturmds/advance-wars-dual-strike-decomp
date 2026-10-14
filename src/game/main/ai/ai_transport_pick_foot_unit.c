#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gAiTransportsSentToUnit[];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Ai_HasEnemyWithin(s16 x, s16 y, int radius);
s16 AiActingUnit_GetSearchMovement(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* For a land transport: picks an own Infantry or Mech it can reach this turn
 * that no other transport has claimed (gAiTransportsSentToUnit), preferring
 * the one it already chose and foot units with an enemy within 5 tiles
 * (Ai_HasEnemyWithin). Claims the foot unit, stores its slot in the
 * transport's aiTargetX and writes the tile to move to. If the foot unit has a target of its own, the tile is the one
 * the transport reaches that is closest to that target. */
BOOL AiTransport_PickFootUnit(s16* tile) {
  MapUnit* unit;
  int col;
  int cost;
  int bestX;
  int row;
  int bestY;
  int bestCost;
  int slot;
  int x;
  int y;
  bestX = -1;
  bestY = -1;
  bestCost = 0x7fff;

  if (Ov000_gUnitStats[STATE->actingUnit->type].moveGroup & MOVE_GROUP_SEA) {
    return FALSE;
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, AiActingUnit_GetSearchMovement(),
                     0);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      slot = STATE->mapTiles->unitAt[TILE(x, y)];
      if (slot == 0 || !Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      if (gAiTransportsSentToUnit[slot & 63] != 0) {
        continue;
      }
      unit = &STATE->units[slot];
      if (unit->type > UNIT_MECH) {
        continue;
      }
      cost = STATE->rangeMap[y][x];
      if ((slot & 63) != STATE->actingUnit->aiTargetX || !unit->unk_0c_6) {
        cost += 256;
      }
      if (!Ai_HasEnemyWithin(x, y, 5)) {
        cost += 512;
      }
      if (bestCost >= cost) {
        bestX = x;
        bestY = y;
        bestCost = cost;
      }
    }
  }
  if (bestCost == 0x7fff) {
    STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
    return FALSE;
  }
  gAiTransportsSentToUnit[STATE->mapTiles->unitAt[TILE(bestX, bestY)] & 63]++;
  STATE->actingUnit->aiTargetX =
      STATE->mapTiles->unitAt[TILE(bestX, bestY)] & 63;
  unit = &STATE->units[STATE->mapTiles->unitAt[TILE(bestX, bestY)]];
  unit->unk_0c_6 = 1;
  if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
    RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
    RangeMap_FillMoves(unit->aiTargetX, unit->aiTargetY,
                       STATE->actingUnit->type, 110, 0);
    RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
    RangeMap_FillMoves(bestX, bestY, STATE->actingUnit->type,
                       gAiTurnState->unk_05, 0);
    /* needed to match: the last search reuses bestCost */
    bestCost = STATE->mapTiles->rangeScratch[TILE(bestX, bestY)];
    if (bestCost >= 0) {
      for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
          if (STATE->rangeMap[row][col] < 0) {
            continue;
          }
          cost = STATE->mapTiles->rangeScratch[TILE(col, row)];
          if (bestCost >= cost) {
            bestX = col;
            bestY = row;
            bestCost = cost;
          }
        }
      }
    }
  }
  tile[0] = bestX;
  tile[1] = bestY;
  return TRUE;
}
