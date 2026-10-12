#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL AiUnit_NeedsService(void);
void func_020b81a8(void);
void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
BOOL Ai_CanEndMoveAt(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Runs when AiUnit_NeedsService says the acting unit needs repair (guess from its
 * code: hp 90 or less, or low fuel). Like AiUnit_MoveToSafestTile, it moves the
 * unit to the free reachable tile with the lowest threat (MapTiles.threatMax
 * + 2 * threatSum), ties to the lower rangeMap value, but only to tiles where
 * Unit_CanRepairAt holds or Unit_IsBlackHoleOnFlaggedTile does. With AI_STEP_UNIT_AT_TARGET
 * set it ignores tiles with a negative MapTiles.rangeScratch and sets AI_STEP_STAY
 * instead of moving when the best tile is where the unit already stands. */
void AiUnit_RetreatToRepair(void) {
  int bestX;
  int bestY;
  int bestThreat;
  int bestReach;
  int threat;
  int x;
  int y;
  int reach;
  int tile;

  bestX = -1;
  bestY = bestX;
  bestThreat = 0x7fff;
  bestReach = bestThreat;
  if (!AiUnit_NeedsService()) {
    return;
  }
  func_020b81a8();
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) {
        if (STATE->mapTiles->rangeScratch[TILE(x, y)] < 0) {
          STATE->rangeMap[y][x] = -1;
          continue;
        }
      }
      if (!Unit_CanRepairAt(STATE->actingUnit, x, y)) {
        if (!Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y)) {
          continue;
        }
      }
      if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
        continue;
      }
      if (Ai_IsTileNeededByAlly(x, y) == 1) {
        continue;
      }
      if (!Ai_CanEndMoveAt(x, y)) {
        continue;
      }
      tile = TILE(x, y);
      threat = STATE->mapTiles->threatMax[tile] +
               STATE->mapTiles->threatSum[tile] * 2;
      reach = STATE->rangeMap[y][x];
      if (threat > bestThreat) {
        continue;
      }
      if (threat == bestThreat && reach > bestReach) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestThreat = threat;
      bestReach = reach;
    }
  }
  if (bestX == -1) {
    return;
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      bestX == STATE->actingUnit->x && bestY == STATE->actingUnit->y) {
    gAiTurnState->stepFlags |= AI_STEP_STAY;
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
}
