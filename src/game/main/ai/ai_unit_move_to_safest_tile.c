#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Ai_CanEndMoveAt(s16 x, s16 y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
void AiUnit_RetreatToRepair(void);
int Unit_GetTerrainStars(int player, int unitType,
                         int terrain); /* terrain defence stars */
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Moves the acting unit to the free tile it can reach with the lowest
 * threat (MapTiles.threatMax + 2 * threatSum). Ties go to more terrain stars,
 * then to the lower rangeMap value. With AI_STEP_UNIT_AT_TARGET set, tiles
 * with a negative MapTiles.rangeScratch get -1 in rangeMap, and nothing is
 * queued when the best tile is the one the unit stands on. */
void AiUnit_MoveToSafestTile(void) {
  int bestX;
  int bestY;
  int bestThreat;
  int bestStars;
  int bestReach;
  int threat;
  int x;
  int y;
  int stars;
  int reach; /* rangeMap value of the tile */
  int tile;

  bestX = -1;
  bestY = bestX;
  bestThreat = 0x7fff;
  bestReach = bestThreat;
  bestStars = bestX;
  if (!(gAiTurnState->stepFlags & AI_STEP_IGNORE_TERRAIN)) {
    AiUnit_RetreatToRepair();
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      (gAiTurnState->stepFlags & AI_STEP_STAY)) {
    return;
  }
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
      if (gAiTurnState->stepFlags & AI_STEP_IGNORE_TERRAIN) {
        stars = 0;
      } else {
        stars = Unit_GetTerrainStars(
            STATE->currentPlayer, STATE->actingUnit->type,
            MAP_TILE_TERRAIN(STATE->mapTiles->tiles[tile]));
      }
      reach = STATE->rangeMap[y][x];
      if (threat > bestThreat) {
        continue;
      }
      if (threat == bestThreat && stars < bestStars) {
        continue;
      }
      if (threat == bestThreat && stars == bestStars && reach > bestReach) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestThreat = threat;
      bestStars = stars;
      bestReach = reach;
    }
  }
  if (bestX == -1) {
    return;
  }
  if ((gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET) &&
      bestX == STATE->actingUnit->x && bestY == STATE->actingUnit->y) {
    return;
  }
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(bestX, bestY, 2, 0,
                       0); /* action 2: move and wait (guess) */
}
