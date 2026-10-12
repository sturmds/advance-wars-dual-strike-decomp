#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/ai_profile.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

void AiUnit_TryAttackBeforeRepair(void);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
BOOL AiUnit_HasFuelForTile(s16 x, s16 y, int moves);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) ((x) + MAP->rowStart[y])

/* Picks a free tile where Unit_CanRepairAt or Unit_IsBlackHoleOnFlaggedTile holds and moves
 * the acting unit there, or toward it when the tile is out of reach this
 * turn. The score starts as the rangeMap value, plus the unit's moves on base,
 * airport and port tiles, and 0 on tiles Unit_IsBlackHoleOnFlaggedTile accepts. Tiles in reach
 * whose threat is above the unit profile's limits are skipped. The lowest score
 * is picked, the last on ties. gAiTurnState->unk_05 holds the unit's moves this turn
 * (see AiTurn_RunQueue). The name is a guess. */
void AiUnit_HeadForRepairTile(void) {
  int bestX;
  int bestY;
  int bestScore;
  int score;
  int x;
  int y;
  int reach;
  int tile;

  AiUnit_TryAttackBeforeRepair();
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, 110, 1);
  bestY = -1;
  bestX = bestY;
  bestScore = 0x7fff;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Unit_CanRepairAt(STATE->actingUnit, x, y)) {
        if (!Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y)) {
          continue;
        }
      }
      if (MAP->unitAt[TILE(x, y)] != 0) {
        continue;
      }
      if (!AiUnit_HasFuelForTile(x, y, gAiTurnState->unk_05)) {
        continue;
      }
      reach = STATE->rangeMap[y][x];
      score = reach;
      if (gTerrainFactoryKind[MAP_TILE_TERRAIN(MAP->tiles[TILE(x, y)])] != 0) {
        score = reach + gAiTurnState->unk_05;
      }
      if (reach <= gAiTurnState->unk_05) {
        if (Ai_IsTileNeededByAlly(x, y) == 1) {
          continue;
        }
        tile = TILE(x, y);
        if (MAP->threatMax[tile] > gAiActiveUnitProfile->threatMaxLimit) {
          continue;
        }
        if (MAP->threatSum[tile] > gAiActiveUnitProfile->threatSumLimit) {
          continue;
        }
        if (Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == 1) {
          score = 0;
        }
      }
      if (score <= bestScore) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  Ai_ClearActingUnitTarget();
  if (STATE->rangeMap[bestY][bestX] <= gAiTurnState->unk_05) {
    AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
    return;
  }
  if (Unit_CanRepairAt(STATE->actingUnit, STATE->actingUnit->x,
                       STATE->actingUnit->y) == 1 &&
      (gAiTurnState->stepFlags & AI_STEP_NO_ADJUST)) {
    AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y, 2, 0, 0);
    return;
  }
  AiUnit_MoveTowardTile(bestX, bestY);
}
