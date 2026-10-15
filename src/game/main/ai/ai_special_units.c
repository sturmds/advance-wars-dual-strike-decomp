#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"
#include "game/map_unit.h"
#include "game/ai/ai_unit_move.h"
#include "game/ai/ai_special_units.h"
#include "game/ai/ai_perform_action.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* Movement for RangeMap_FillMoves high enough to reach the whole map. */
#define RANGE_WHOLE_MAP 110
#define NO_TILE_POS 9999 /* an x or y no tile has: none found */
/* rangeMap marks RangeMap_MarkAdjacentTiles writes, 110 to 112. */
#define BLAST_MARK_FIRST 110
#define BLAST_MARK_COUNT 3
/* Radius of a Black Bomb explosion in tiles. */
#define BLOMB_RADIUS 3
/* Each player owns MAP_UNITS_PER_PLAYER (64) unit slots, so slot >> 6 is the
 * owner's index minus one. Written as a shift to match. */
#define SLOT_OWNER_SHIFT 6

void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
BOOL Player_IsInGame(u8 player);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(s8 mark);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int Map_GetDistance(int x1, int y1, int x2, int y2);
int UnitType_GetAttackKind(u8 unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);

/* Score of one unit hit by a Black Bomb explosion: the value of the hp it
 * would lose (at most 50 hp, never the last one), negative when the unit
 * is on the team of the acting Black Bomb. */
int AiBlackBomb_ScoreUnit(MapUnit* unit) {
  int value;
  u16 loss;
  int unitOwner;
  int bombOwner;

  value = Unit_GetValuePerHp(MAP_STATE->currentPlayer, unit->type);
  loss = unit->hp;
  if (loss > 50) {
    loss = 50;
  } else {
    loss--;
  }
  unitOwner = (unit - MAP_STATE->units) >> SLOT_OWNER_SHIFT;
  bombOwner = MAP_STATE->actingUnitIndex >> SLOT_OWNER_SHIFT;
  value *= loss;
  if (MAP_STATE->players[bombOwner + 1].team ==
      (&MAP_STATE->players[unitOwner])[1].team) {
    return -value;
  }
  return value;
}

/* Score a Black Bomb gives for exploding: half the value of its hp. Map
 * 226 always gives 1. */
u32 AiBlackBomb_GetStartScore(void) {
  int value;

  if (MAP_STATE->unk_1c4->mapId == 226) {
    return 1;
  }
  value = Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_BLACK_BOMB);
  return (u32)(value * MAP_STATE->actingUnit->hp) / 2;
}

/* Adds up AiBlackBomb_ScoreUnit over the units within 3 tiles (by taxicab
 * distance, not on the centre tile) of (x, y). Skipped: Black Bombs,
 * Oozium, unarmed units, units at 10 hp or less and tiles out of sight. */
int AiBlackBomb_ScoreExplosionAt(int x, int y) {
  /* needed to match: declaration order sets the register choice */
  int maxY;
  int dist;
  int ty;
  MapUnit* unit;
  int maxX;
  int total;
  int minX;
  int tx;
  int minY;

  total = 0;
  minX = x - BLOMB_RADIUS;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - BLOMB_RADIUS;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + BLOMB_RADIUS < maxX) {
    maxX = x + BLOMB_RADIUS;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + BLOMB_RADIUS < maxY) {
    maxY = y + BLOMB_RADIUS;
  }
  for (ty = minY; ty <= maxY; ty++) {
    for (tx = minX; tx <= maxX; tx++) {
      dist = 0;
      if (x - tx > 0) {
        dist += x - tx;
      } else {
        dist += tx - x;
      }
      if (y - ty > 0) {
        dist += y - ty;
      } else {
        dist += ty - y;
      }
      if (dist < 1 || dist > BLOMB_RADIUS) {
        continue;
      }
      unit = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(tx, ty)]];
      if (unit->type == UNIT_NONE || unit->type == UNIT_OOZIUM ||
          unit->type == UNIT_BLACK_BOMB) {
        continue;
      }
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_NONE) {
        continue;
      }
      if (unit->hp <= 10) {
        continue;
      }
      if (!Map_IsTileVisible(unit->x, unit->y)) {
        continue;
      }
      total += AiBlackBomb_ScoreUnit(unit);
    }
  }
  return total;
}

/* Scores every empty tile the acting Black Bomb can reach with
 * AiBlackBomb_ScoreExplosionAt and acts on the last tile that scores at least as much
 * as the start value: AiBlackBomb_GetStartScore, or 1 when the bomb has under 10
 * fuel. Action 23 is the explosion (guess from the caller). */
void AiBlackBomb_ChooseExplosionTile(void) {
  int bestX;
  int bestY;
  int bestScore;
  int score;
  int x;
  int y;

  bestX = -1;
  bestY = bestX;
  bestScore = AiBlackBomb_GetStartScore();
  if (MAP_STATE->actingUnit->fuel < 10) {
    bestScore = 1;
  }
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] != 0) {
        continue;
      }
      score = AiBlackBomb_ScoreExplosionAt(x, y);
      if (score >= bestScore) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestX == -1) {
    return;
  }
  AiUnit_PerformAction(bestX, bestY, 23, 0, 0);
}

/* Moves the acting Black Bomb toward an enemy unit it cannot reach this
 * turn. Prefers the nearest such unit that is not carried, has over 60 hp
 * and costs at least a quarter of a Black Bomb per hp. Without one, goes
 * for the nearest enemy unit. The marks 110 to 112 probably cover the
 * tiles next to the reachable area, so a negative rangeMap value
 * means out of reach. */
void AiBlackBomb_MoveTowardEnemy(void) {
  int valuableX;
  int valuableY;
  int nearestX;
  int nearestY;
  int i;
  int bestDist;
  int bestValuableDist;
  int player;
  int slot;
  MapUnit* unit;
  int dist;

  valuableX = -1;
  valuableY = valuableX;
  nearestX = valuableX;
  nearestY = valuableX;
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (i = 0; i < BLAST_MARK_COUNT; i++) {
    RangeMap_MarkAdjacentTiles(BLAST_MARK_FIRST + i);
  }
  bestDist = 0x7fff;
  bestValuableDist = 0x7fff;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
        MAP_STATE->players[player].team) {
      continue;
    }
    for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
         slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
      unit = &MAP_STATE->units[slot];
      if (unit->type == UNIT_NONE) {
        continue;
      }
      dist = Map_GetDistance(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                             unit->x, unit->y);
      if (bestDist > dist) {
        bestDist = dist;
        nearestX = unit->x;
        nearestY = unit->y;
      }
      if (MAP_STATE->rangeMap[unit->y][unit->x] >= 0) {
        continue;
      }
      if (unit->flags & MAP_UNIT_CARRIED) {
        continue;
      }
      if (unit->hp < 61) {
        continue;
      }
      if (Unit_GetValuePerHp(MAP_STATE->currentPlayer, unit->type) <
          Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_BLACK_BOMB) >> 2) {
        continue;
      }
      dist = Map_GetDistance(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                             unit->x, unit->y);
      if (bestValuableDist > dist) {
        valuableX = unit->x;
        valuableY = unit->y;
        bestValuableDist = dist;
      }
    }
  }
  if (bestValuableDist == 0x7fff) {
    if (bestDist == 0x7fff) {
      return;
    }
    AiUnit_MoveTowardTile(nearestX, nearestY);
  } else {
    AiUnit_MoveTowardTile(valuableX, valuableY);
  }
}

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
  RangeMap_FillMoves(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                     MAP_STATE->actingUnit->type, RANGE_WHOLE_MAP, 0);
  bestX = NO_TILE_POS;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (MAP_STATE->rangeMap[y][x] > bestDist) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      unit = &MAP_STATE->units[slot];
      if (unit->type == 0) {
        continue;
      }
      /* (slot >> 6) + 1: the unit owner's player slot */
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      value = unit->hp * Ov000_gUnitStats[unit->type].costDiv10;
      if (bestDist == MAP_STATE->rangeMap[y][x]) {
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
      bestDist = MAP_STATE->rangeMap[y][x];
      bestValue = value;
      bestX = x;
      bestY = y;
    }
  }
  if (bestX == NO_TILE_POS) {
    return;
  }

  RangeMap_FillMoves(bestX, bestY, MAP_STATE->actingUnit->type, RANGE_WHOLE_MAP,
                     0);
  stepX = NO_TILE_POS;
  unitX = MAP_STATE->actingUnit->x;
  unitY = MAP_STATE->actingUnit->y;
  value = MAP_STATE->rangeMap[unitY][unitX];
  minX = unitX - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = unitY - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (unitX + 1 < maxX) {
    maxX = unitX + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (unitY + 1 < maxY) {
    maxY = unitY + 1;
  }
  for (tileY = minY; tileY <= maxY; tileY++) {
    for (tileX = minX; tileX <= maxX; tileX++) {
      steps = 0; /* tiles from the Oozium; only the 4 next to it count */
      if (MAP_STATE->actingUnit->x - tileX > 0) {
        steps += MAP_STATE->actingUnit->x - tileX;
      } else {
        steps += tileX - MAP_STATE->actingUnit->x;
      }
      if (MAP_STATE->actingUnit->y - tileY > 0) {
        steps += MAP_STATE->actingUnit->y - tileY;
      } else {
        steps += tileY - MAP_STATE->actingUnit->y;
      }
      if (steps < 1 || steps > 1) {
        continue;
      }
      bestDist = MAP_STATE->rangeMap[tileY][tileX];
      if (bestDist < 0 || bestDist > value) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(tileX, tileY)];
      if (MAP_STATE->units[slot].type != 0) {
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
            MAP_STATE->players[(slot >> 6) + 1].team) {
          continue;
        }
      } else {
        if (MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(tileX, tileY)] != 0) {
          continue;
        }
        if (MAP_STATE->mapTiles->threatSum[MAP_TILE_INDEX(tileX, tileY)] != 0) {
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
  if (stepX == NO_TILE_POS) {
    return;
  }
  if (MAP_STATE
          ->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(stepX, stepY)]]
          .type != 0) {
    AiUnit_PerformAction(stepX, stepY, AI_ACTION_OOZIUM_EAT, 0, 0);
  } else {
    AiUnit_PerformAction(stepX, stepY, AI_ACTION_WAIT, 0, 0);
  }
}
