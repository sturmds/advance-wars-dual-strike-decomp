#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int GetUnitMaxRange(int player, int unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(int target, s16* pos);
void AiUnit_PerformAction(int x, int y, int action, u8 arg3, u8 arg4);
void AiUnit_MoveToSafestTile(void);
void RangeMap_FillUnitMoves(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* Run by AiUnit_HandleUnitAtTarget for indirect units. The acting unit
 * attacks from where it stands: the best-scored visible enemy unit in its
 * range gets action 4 (like Ai_PlanIndirectAttack, but without map objects
 * or hp tiles, and with a fixed score flag set). Then the move range of
 * mover is filled in (the range map points at MapTiles.rangeScratch meanwhile)
 * and AiUnit_MoveToSafestTile runs. */
void AiUnit_AttackFromPlaceThenMove(MapUnit* mover) {
  /* needed to match: declaration order sets the register choice */
  int maxX;
  MapUnit* target;
  int y;
  int distance;
  int score;
  int x;
  int maxY;
  int minY;
  int bestSlot;
  int bestScore;
  int minX;
  s16 pos[2];

  bestScore = 0;
  bestSlot = 0;
  pos[0] = STATE->actingUnit->x;
  pos[1] = STATE->actingUnit->y;
  if (STATE->actingUnit->x -
          GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) <
      0) {
    minX = 0;
  } else {
    minX = STATE->actingUnit->x -
           GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
  }
  if (STATE->actingUnit->y -
          GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) <
      0) {
    minY = 0;
  } else {
    minY = STATE->actingUnit->y -
           GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (STATE->actingUnit->x +
          GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) <
      maxX) {
    maxX = STATE->actingUnit->x +
           GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (STATE->actingUnit->y +
          GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type) <
      maxY) {
    maxY = STATE->actingUnit->y +
           GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type);
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      distance = 0;
      if (STATE->actingUnit->x - x > 0) {
        distance += STATE->actingUnit->x - x;
      } else {
        distance += x - STATE->actingUnit->x;
      }
      if (STATE->actingUnit->y - y > 0) {
        distance += STATE->actingUnit->y - y;
      } else {
        distance += y - STATE->actingUnit->y;
      }
      if (distance < Ov000_gUnitStats[STATE->actingUnit->type].minRange) {
        continue;
      }
      if (distance >
          GetUnitMaxRange(STATE->currentPlayer, STATE->actingUnit->type)) {
        continue;
      }
      target = &STATE->units[MAP->unitAt[TILE_INDEX(x, y)]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
        continue;
      }
      if (Map_IsTileVisible(x, y) == 0) {
        continue;
      }
      score = AiAttack_Score(AI_ATTACK_HEAVY_HIT | AI_ATTACK_SAFE |
                                 MAP->unitAt[TILE_INDEX(target->x, target->y)],
                             pos);
      if (score != 0 && score >= bestScore) {
        bestScore = score;
        bestSlot = MAP->unitAt[TILE_INDEX(x, y)];
      }
    }
  }
  if (bestScore != 0) {
    AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y,
                         AI_ACTION_ATTACK_UNIT, bestSlot, 0);
  }
  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillUnitMoves(mover);
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  AiUnit_MoveToSafestTile();
}
