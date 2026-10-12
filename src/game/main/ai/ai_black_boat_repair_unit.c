#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL FindAttackTile(int x, int y, s16* pos);
BOOL Tile_HasUnitToRepair(s16 x, s16 y);
int Tile_GetDirection(
    int fromX, int fromY, int toX,
    int toY); /* direction from the first tile to the second: 0 same tile, 1 up, 2 right, 3 down, 4 left */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* MapUnit.serviceWanted value that AiUnit_SeekSupply and func_020af5f8 act on;
 * this routine clears it once it repairs the unit. */
#define WANTS_SUPPLY 1

/* Same search as AiClass3_SupplyUnit, with Tile_HasUnitToRepair as the filter:
 * picks the unit in reach with WANTS_SUPPLY set and the least fuel, and
 * performs the repair action from an adjacent tile. Called by AiBlackBoat_Act. */
void AiBlackBoat_RepairUnit(void) {
  MapUnit* unit;
  MapUnit* bestUnit = NULL;
  int bestX = -1;
  int bestY = -1;
  int bestFuel = 100;
  int x;
  int y;
  s16 pos[2];

  RangeMap_FillUnitMoves(STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      unit = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      if (Tile_HasUnitToRepair(x, y) && unit->serviceWanted == WANTS_SUPPLY &&
          bestFuel >= unit->fuel) {
        if (FindAttackTile(x, y, pos)) {
          bestFuel = unit->fuel;
          bestX = pos[0];
          bestY = pos[1];
          bestUnit = unit;
        }
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  bestUnit->serviceWanted = 0;
  AiUnit_PerformAction(
      bestX, bestY, AI_ACTION_REPAIR,
      Tile_GetDirection(bestX, bestY, bestUnit->x, bestUnit->y), 0);
}
