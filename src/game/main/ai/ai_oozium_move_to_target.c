#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Movement for RangeMap_FillMoves high enough to reach the whole map. */
#define RANGE_WHOLE_MAP 110

#define NO_TILE 9999

/* Ooziums: picks the nearest enemy unit by move cost, the one worth the
 * most (hp * cost) on a tie, then by the roll. Then steps one tile toward
 * it: onto it when next to it, else to a free tile no enemy threatens. */
void AiOozium_MoveToTarget(void) {
  /* Needed to match: declaration order sets the registers; the first
     * search reads the move cost from rangeMap at each use, and after it
     * value holds the best step's cost and bestDist the tile's. */
  int bestValue;
  int unitX;
  int minY;
  int unitY;
  int slot;
  s16 bestY;
  int x;
  MapUnit* unit;
  int tileY;
  int steps;
  int y;
  int maxY;
  s16 stepX;
  int tileX;
  int maxX;
  s16 stepY;
  int bestDist;
  int value;
  s16 bestX;
  int minX;

  bestValue = 0;
  bestDist = 255;
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, RANGE_WHOLE_MAP, 0);
  bestX = NO_TILE;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (STATE->rangeMap[y][x] > bestDist) {
        continue;
      }
      slot = STATE->mapTiles->unitAt[TILE(x, y)];
      unit = &STATE->units[slot];
      if (unit->type == 0) {
        continue;
      }
      /* (slot >> 6) + 1: the unit owner's player slot */
      if (STATE->players[STATE->currentPlayer].team ==
          STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      value = unit->hp * Ov000_gUnitStats[unit->type].costDiv10;
      if (bestDist == STATE->rangeMap[y][x]) {
        if (bestValue > value) {
          continue;
        }
        if (bestValue == value) {
          if (gAiTurnState->roll < 25) {
            if (bestY < y) {
              continue;
            }
            if (bestY == y && bestX < x) {
              continue;
            }
          } else if (gAiTurnState->roll < 50) {
            if (bestX > x) {
              continue;
            }
            if (bestX == x && bestY < y) {
              continue;
            }
          } else if (gAiTurnState->roll < 75) {
            if (bestX < x) {
              continue;
            }
            if (bestX == x && bestY > y) {
              continue;
            }
          } else {
            if (bestY > y) {
              continue;
            }
            if (bestY == y && bestX > x) {
              continue;
            }
          }
        }
      }
      bestDist = STATE->rangeMap[y][x];
      bestValue = value;
      bestX = x;
      bestY = y;
    }
  }
  if (bestX == NO_TILE) {
    return;
  }

  RangeMap_FillMoves(bestX, bestY, STATE->actingUnit->type, RANGE_WHOLE_MAP, 0);
  stepX = NO_TILE;
  unitX = STATE->actingUnit->x;
  unitY = STATE->actingUnit->y;
  value = STATE->rangeMap[unitY][unitX];
  minX = unitX - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = unitY - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (unitX + 1 < maxX) {
    maxX = unitX + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (unitY + 1 < maxY) {
    maxY = unitY + 1;
  }
  for (tileY = minY; tileY <= maxY; tileY++) {
    for (tileX = minX; tileX <= maxX; tileX++) {
      steps = 0; /* tiles from the Oozium; only the 4 next to it count */
      if (STATE->actingUnit->x - tileX > 0) {
        steps += STATE->actingUnit->x - tileX;
      } else {
        steps += tileX - STATE->actingUnit->x;
      }
      if (STATE->actingUnit->y - tileY > 0) {
        steps += STATE->actingUnit->y - tileY;
      } else {
        steps += tileY - STATE->actingUnit->y;
      }
      if (steps < 1 || steps > 1) {
        continue;
      }
      bestDist = STATE->rangeMap[tileY][tileX];
      if (bestDist < 0 || bestDist > value) {
        continue;
      }
      slot = STATE->mapTiles->unitAt[TILE(tileX, tileY)];
      if (STATE->units[slot].type != 0) {
        if (STATE->players[STATE->currentPlayer].team ==
            STATE->players[(slot >> 6) + 1].team) {
          continue;
        }
      } else {
        if (STATE->mapTiles->threatMax[TILE(tileX, tileY)] != 0) {
          continue;
        }
        if (STATE->mapTiles->threatSum[TILE(tileX, tileY)] != 0) {
          continue;
        }
      }
      if (value == bestDist) {
        if (gAiTurnState->roll < 25) {
          if (stepY < tileY) {
            continue;
          }
          if (stepY == tileY && stepX < tileX) {
            continue;
          }
        } else if (gAiTurnState->roll < 50) {
          if (stepX > tileX) {
            continue;
          }
          if (stepX == tileX && stepY < tileY) {
            continue;
          }
        } else if (gAiTurnState->roll < 75) {
          if (stepX < tileX) {
            continue;
          }
          if (stepX == tileX && stepY > tileY) {
            continue;
          }
        } else {
          if (stepY > tileY) {
            continue;
          }
          if (stepY == tileY && stepX > tileX) {
            continue;
          }
        }
      }
      value = bestDist;
      stepX = tileX;
      stepY = tileY;
    }
  }
  if (stepX == NO_TILE) {
    return;
  }
  if (STATE->units[STATE->mapTiles->unitAt[TILE(stepX, stepY)]].type != 0) {
    AiUnit_PerformAction(stepX, stepY, AI_ACTION_OOZIUM_EAT, 0, 0);
  } else {
    AiUnit_PerformAction(stepX, stepY, AI_ACTION_WAIT, 0, 0);
  }
}
