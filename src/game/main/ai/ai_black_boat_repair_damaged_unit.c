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
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int Tile_GetDirection(int fromX, int fromY, int toX, int toY);
BOOL AiBlackBoat_IsBetterRepairTarget(MapUnit* best, MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define NO_TILE 9999
#define FULL_SHOWN_HP_MIN 91 /* hp 91-100 shows as 10 */

/* Run by AiSeaTransport_GoToFootUnit for an empty Black Boat. Among the
 * current player's units it can reach next to (not Oozium, not itself)
 * with less than 10 shown HP, and whose one-HP repair cost
 * (Unit_GetValuePerHp) the player can pay, it keeps the one
 * AiBlackBoat_IsBetterRepairTarget prefers and repairs it from an adjacent tile. */
void AiBlackBoat_RepairDamagedUnit(void) {
  /* declaration order needed to match (register allocation) */
  int tile;
  MapUnit* unit;
  s16 bestX;
  MapUnit* bestUnit;
  s16 pos[2];
  int x;
  int y;
  s16 bestY;

  bestUnit = NULL;
  RangeMap_FillUnitMoves(STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  bestX = NO_TILE;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = TILE(x, y);
      unit = &STATE->units[STATE->mapTiles->unitAt[tile]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(STATE->mapTiles->unk_508[tile])) {
        continue;
      }
      if (unit->hp >= FULL_SHOWN_HP_MIN) {
        continue;
      }
      if (unit == STATE->actingUnit) {
        continue;
      }
      if ((u32)STATE->players[STATE->currentPlayer].funds <
          (u32)Unit_GetValuePerHp(STATE->currentPlayer, unit->type)) {
        continue;
      }
      if (AiBlackBoat_IsBetterRepairTarget(bestUnit, unit) != TRUE) {
        continue;
      }
      if (FindAttackTile(x, y, pos)) {
        bestX = pos[0];
        bestY = pos[1];
        bestUnit = unit;
      }
    }
  }
  if (bestX == NO_TILE) {
    return;
  }
  AiUnit_PerformAction(
      bestX, bestY, AI_ACTION_REPAIR,
      Tile_GetDirection(bestX, bestY, bestUnit->x, bestUnit->y), 0);
}
