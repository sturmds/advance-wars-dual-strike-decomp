#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(s8 mark);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiUnit_TeammateOutdamages(MapUnit* target);
BOOL FindAttackTile(int x, int y, s16* pos);
int AiAttack_Score(int arg, s16* pos);
void AiUnit_MoveToSafestTile(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* AiUnit_HandleUnitAtTarget calls this for two of the AI's units: the acting
 * unit and other. Puts other's reach (RangeMap_FillUnitMoves) in
 * MapTiles.rangeScratch, then picks a direct attack for the acting unit like
 * AiUnit_PickDirectAttack, but only from tiles both units can reach, only
 * against units, and always passing AI_ATTACK_HEAVY_HIT | AI_ATTACK_SAFE to
 * AiAttack_Score. Ends with
 * AiUnit_MoveToSafestTile, with rangeScratch holding other's reach again. */
void AiUnit_PickDirectAttackInReachOf(MapUnit* other) {
  int maxY;
  int minX;
  int bestX;
  int bestY;
  int bestScore;
  int score;
  int targetSlot;
  int x;
  int y;
  MapUnit* target;
  MapUnit* neighbour;
  s16 pos[2];
  int maxX;
  int minY;
  int openSides;
  int xx;
  int yy;
  int dist;
  int col; /* needed to match: the first loop has its own counters */
  int row;

  bestX = -1;
  bestScore = 0;
  bestY = bestX;
  targetSlot = bestScore;
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  RangeMap_FillUnitMoves(other);
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
      if (STATE->rangeMap[row][col] >= 0 &&
          STATE->mapTiles->rangeScratch[TILE(col, row)] < 0) {
        STATE->rangeMap[row][col] = -1;
      }
    }
  }
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      target = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
      if (target->type == 0) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      score = AiAttack_Score(STATE->mapTiles->unitAt[TILE(x, y)] |
                                 (AI_ATTACK_HEAVY_HIT | AI_ATTACK_SAFE),
                             pos);
      if (score == 0 || score < bestScore) {
        continue;
      }
      if (AiUnit_TeammateOutdamages(target) == 1) {
        /* count the target's free sides; with one or none left, skip it */
        openSides = 0;
        minX = target->x - 1;
        if (minX < 0) {
          minX = 0;
        }
        minY = target->y - 1;
        if (minY < 0) {
          minY = 0;
        }
        maxX = STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = STATE->unk_1c4->mapHeight - 1;
        if (target->y + 1 < maxY) {
          maxY = target->y + 1;
        }
        for (yy = minY; yy <= maxY; yy++) {
          for (xx = minX; xx <= maxX; xx++) {
            dist = 0;
            if (target->x - xx > 0) {
              dist += target->x - xx;
            } else {
              dist += xx - target->x;
            }
            if (target->y - yy > 0) {
              dist += target->y - yy;
            } else {
              dist += yy - target->y;
            }
            if (dist < 1 || dist > 1) {
              continue;
            }
            if (STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &STATE->units[STATE->mapTiles->unitAt[TILE(xx, yy)]];
            if (neighbour->type != 0 && (neighbour->flags & MAP_UNIT_WAITED)) {
              continue;
            }
            openSides++;
          }
        }
        if (openSides <= 1) {
          continue;
        }
      }
      bestScore = score;
      bestX = pos[0];
      bestY = pos[1];
      targetSlot = STATE->mapTiles->unitAt[TILE(x, y)];
    }
  }
  if (bestScore != 0) {
    AiUnit_PerformAction(bestX, bestY, 4, targetSlot, 0);
  }
  RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
  RangeMap_FillUnitMoves(other);
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  AiUnit_MoveToSafestTile();
}
