#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/ai/ai_perform_action.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/math.h"
#include "game/map_unit.h"
#include "game/co_data.h"
#include "game/ai/ai_unit_target.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_tile_search.h"
#include "game/ai/ai_unit_move.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define MAX_RANGE() \
  GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type)
/* rebuilds the AI threat maps */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define MOVE_RANGE(unit) \
  Player_GetUnitMoveRange(MAP_UNIT_OWNER(MAP_STATE, unit), (unit)->type)
#define MOVE_COST(unit, x, y)                    \
  chart[Ov000_gUnitStats[(unit)->type].moveType] \
       [MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)])]
#define PLAYER_OF_SLOT(slot) (MAP_STATE->players[((slot) >> 6) + 1])
/* AiUnit_PerformAction actions. Names of 2, 11 and 12 are guesses from
 * when this function picks them. */
#define AI_ACTION_WAIT 2
#define AI_ACTION_CAPTURE 3
#define AI_ACTION_SUPPLY 6
#define AI_ACTION_UNLOAD \
  8 /* target, arg: direction of each cargo unit, 1 up, 2 right, 3 down, 4 left */
#define AI_ACTION_HIDE 11
#define AI_ACTION_APPEAR 12
#define AI_ACTION_REPAIR 24 /* target: direction of the unit */
/* Unload tiles with more threat than this (threatMax + threatSum) are skipped. */
#define UNLOAD_MAX_THREAT 40

int UnitType_GetAttackKind(u8 unitType);
int GetUnitMaxRange(int player, int unitType);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiAttack_Score(u32 flags, s16* pos);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(s8 mark);
int Rng_Next(void);
u8 Unit_GetUsableMovement(MapUnit* unit);
int Map_IsCapturableByActingUnit(s16 x, s16 y);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Unit_GetCoMaxRange(MapUnit* unit);
/* max range with the CO's range bonus */
int Player_GetUnitMoveRange(int player, int unitType);
int UnitType_BestBaseDamage(int unitType, int targetType);
BOOL Tile_HasUnitToSupply(s16 x, s16 y);
/* another unit of the current player on (x, y) short of ammo or fuel */
BOOL Tile_HasUnitToRepair(s16 x, s16 y);
/* the same, or below 10 shown hp; never Oozium */
/* direction from (x, y) to (toX, toY): 1 up, 2 right, 3 down, 4 left, 0 same */
void func_0209c1f8(void);

/* Run by AiUnit_PerformAction before it records an action; the callees
 * may change action, target and arg. With AI_STEP_SWAP_CARGO
 * set, first swaps the acting unit's two cargo slots (and, for action 8,
 * target and arg with them) and clears the bit. AI_STEP_NO_ADJUST skips the rest. */
void AiUnit_AdjustAction(s16 x, s16 y, u8* action, u8* target, u8* arg) {
  u8 swap;

  if (gAiTurnState->stepFlags & AI_STEP_SWAP_CARGO) {
    swap = MAP_STATE->actingUnit->cargo[0];
    MAP_STATE->actingUnit->cargo[0] = MAP_STATE->actingUnit->cargo[1];
    MAP_STATE->actingUnit->cargo[1] = swap;
    if (*action == 8) {
      swap = *target;
      *target = *arg;
      *arg = swap;
    }
    gAiTurnState->stepFlags &= ~AI_STEP_SWAP_CARGO;
  }
  if (gAiTurnState->stepFlags & AI_STEP_NO_ADJUST) {
    return;
  }
  switch (*action) {
    case 2:
      if (!(gAiTurnState->stepFlags & AI_STEP_UNIT_AT_TARGET)) {
        AiUnit_WaitToAttack(x, y, action, target);
      }
      break;
    case 10:
      AiUnit_HandleUnitAtTarget(x, y);
      break;
  }
  AiUnit_UpgradeWaitAction(x, y, action, target, arg);
}

/* Run by AiUnit_AdjustAction on the action the AI picked for its acting
 * unit at (x, y). Replaces a plain wait with what the unit type can do
 * there:
 * - Infantry and Mech capture.
 * - A Sub or Stealth hides when Unit_CanRepairAt or Unit_IsBlackHoleOnFlaggedTile holds
 *   for the tile, or when an enemy within 8 tiles (Stealth: 10) can reach
 *   it and deal 10% or more; with no such enemy, a hidden one appears.
 * - An APC that moved supplies when Tile_HasUnitToSupply finds a unit next to it.
 * - A Black Boat that moved repairs the neighbour with the least fuel.
 * - A Cruiser or Carrier unloads each cargo unit that has full fuel onto a
 *   free, passable, low-threat neighbour tile; the second cargo has to go
 *   in another direction than the first. */
void AiUnit_UpgradeWaitAction(s16 x, s16 y, u8* action, u8* target, u8* arg) {
  /* needed to match: each search has its own variables, while count,
     * unit and chart are shared */
  MapUnit* best = NULL;
  MapUnit* unit;
  int count = 0;
  int unloadEndX;
  int unloadStartY;
  int unloadMaxX;
  int unloadMinX;
  int unloadMinY;
  int unloadX;
  int unloadY;
  int unloadDist;
  int unload2X;
  int unload2Dist;
  int unloadEndY;
  int unloadMaxY;
  int unloadStartX;
  int repairMaxY;
  int repairMaxX;
  int repairMinY;
  int repairMinX;
  int repairX;
  int repairY;
  int repairDist;
  MapUnit* repairTarget;
  int supplyMaxY;
  int supplyMaxX;
  int supplyMinY;
  int supplyMinX;
  int supplyX;
  int supplyY;
  int supplyDist;
  int stealthMaxY;
  int stealthMaxX;
  int stealthMinY;
  int stealthMinX;
  int stealthX;
  int stealthY;
  int stealthDist;
  int stealthRange;
  int stealthSlot;
  int subMaxY;
  int subMaxX;
  int subMinY;
  int subMinX;
  int subX;
  int subY;
  int subDist;
  int subRange;
  int subSlot;
  const s8(*chart)[32]; /* movement cost per move type and terrain */
  MapUnit* secondCargo;

  if (*action != AI_ACTION_WAIT) {
    return;
  }
  switch (MAP_STATE->actingUnit->type) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      if (Map_IsCapturableByActingUnit(x, y) == TRUE) {
        *action = AI_ACTION_CAPTURE;
      }
      return;
    case UNIT_SUB:
      if (Unit_CanRepairAt(MAP_STATE->actingUnit, x, y) == TRUE ||
          Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == TRUE) {
        if (!(MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
          *action = AI_ACTION_HIDE;
        }
        return;
      }
      subMinX = x - 8;
      if (subMinX < 0) {
        subMinX = 0;
      }
      subMinY = y - 8;
      if (subMinY < 0) {
        subMinY = 0;
      }
      subMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (x + 8 < subMaxX) {
        subMaxX = x + 8;
      }
      subMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      if (y + 8 < subMaxY) {
        subMaxY = y + 8;
      }
      for (subY = subMinY; subY <= subMaxY; subY++) {
        for (subX = subMinX; subX <= subMaxX; subX++) {
          subDist = 0;
          if (x - subX > 0) {
            subDist += x - subX;
          } else {
            subDist += subX - x;
          }
          if (y - subY > 0) {
            subDist += y - subY;
          } else {
            subDist += subY - y;
          }
          if (subDist < 1 || subDist > 8) {
            continue;
          }
          subSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(subX, subY)];
          if (subSlot == 0) {
            continue;
          }
          if (PLAYER_OF_SLOT(MAP_STATE->actingUnitIndex).team ==
              PLAYER_OF_SLOT(subSlot).team) {
            continue;
          }
          unit = &MAP_STATE->units[subSlot];
          subRange = Unit_GetCoMaxRange(unit);
          if (subRange > 1) {
            /* ships with indirect fire reach one tile further */
            if (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA) {
              subRange++;
            }
            if (subRange < subDist) {
              continue;
            }
          } else {
            if (MOVE_RANGE(unit) + 1 < subDist) {
              continue;
            }
            if (Ov000_gUnitStats[unit->type].moveGroup &
                (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
              RangeMap_FillUnitMoves(unit);
              RangeMap_MarkAdjacentTiles(111);
              if (MAP_STATE->rangeMap[y][x] < 0) {
                continue;
              }
            }
          }
          if (UnitType_BestBaseDamage(unit->type, UNIT_SUB) >= 10) {
            count++;
          }
        }
      }
      if (count != 0 && !(MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_HIDE;
        return;
      }
      if (count == 0 && (MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_APPEAR;
      }
      return;
    case UNIT_STEALTH:
      if (Unit_CanRepairAt(MAP_STATE->actingUnit, x, y) == TRUE ||
          Unit_IsBlackHoleOnFlaggedTile(MAP_STATE->actingUnit, x, y) == TRUE) {
        if (!(MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
          *action = AI_ACTION_HIDE;
        }
        return;
      }
      stealthMinX = x - 10;
      if (stealthMinX < 0) {
        stealthMinX = 0;
      }
      stealthMinY = y - 10;
      if (stealthMinY < 0) {
        stealthMinY = 0;
      }
      stealthMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (x + 10 < stealthMaxX) {
        stealthMaxX = x + 10;
      }
      stealthMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      if (y + 10 < stealthMaxY) {
        stealthMaxY = y + 10;
      }
      for (stealthY = stealthMinY; stealthY <= stealthMaxY; stealthY++) {
        for (stealthX = stealthMinX; stealthX <= stealthMaxX; stealthX++) {
          stealthDist = 0;
          if (x - stealthX > 0) {
            stealthDist += x - stealthX;
          } else {
            stealthDist += stealthX - x;
          }
          if (y - stealthY > 0) {
            stealthDist += y - stealthY;
          } else {
            stealthDist += stealthY - y;
          }
          if (stealthDist < 1 || stealthDist > 10) {
            continue;
          }
          stealthSlot =
              MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(stealthX, stealthY)];
          if (stealthSlot == 0) {
            continue;
          }
          if (PLAYER_OF_SLOT(MAP_STATE->actingUnitIndex).team ==
              PLAYER_OF_SLOT(stealthSlot).team) {
            continue;
          }
          unit = &MAP_STATE->units[stealthSlot];
          stealthRange = Unit_GetCoMaxRange(unit);
          if (stealthRange > 1) {
            if (stealthRange + 1 < stealthDist) {
              continue;
            }
          } else {
            if (MOVE_RANGE(unit) + 1 < stealthDist) {
              continue;
            }
          }
          if (UnitType_BestBaseDamage(unit->type, UNIT_STEALTH) >= 10) {
            count++;
          }
        }
      }
      if (count != 0 && !(MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_HIDE;
        return;
      }
      if (count == 0 && (MAP_STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_APPEAR;
      }
      return;
    case UNIT_APC:
      if (x == MAP_STATE->actingUnit->x && y == MAP_STATE->actingUnit->y) {
        return;
      }
      count = 0;
      supplyMinX = x - 1;
      if (supplyMinX < 0) {
        supplyMinX = 0;
      }
      supplyMinY = y - 1;
      if (supplyMinY < 0) {
        supplyMinY = 0;
      }
      supplyMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < supplyMaxX) {
        supplyMaxX = x + 1;
      }
      supplyMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      if (y + 1 < supplyMaxY) {
        supplyMaxY = y + 1;
      }
      for (supplyY = supplyMinY; supplyY <= supplyMaxY; supplyY++) {
        for (supplyX = supplyMinX; supplyX <= supplyMaxX; supplyX++) {
          supplyDist = 0;
          if (x - supplyX > 0) {
            supplyDist += x - supplyX;
          } else {
            supplyDist += supplyX - x;
          }
          if (y - supplyY > 0) {
            supplyDist += y - supplyY;
          } else {
            supplyDist += supplyY - y;
          }
          if (supplyDist < 1 || supplyDist > 1) {
            continue;
          }
          if (Tile_HasUnitToSupply(supplyX, supplyY)) {
            count++;
          }
        }
      }
      if (count != 0) {
        *action = AI_ACTION_SUPPLY;
      }
      return;
    case UNIT_BLACK_BOAT:
      if (x == MAP_STATE->actingUnit->x && y == MAP_STATE->actingUnit->y) {
        return;
      }
      repairMinX = x - 1;
      if (repairMinX < 0) {
        repairMinX = 0;
      }
      repairMinY = y - 1;
      if (repairMinY < 0) {
        repairMinY = 0;
      }
      repairMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < repairMaxX) {
        repairMaxX = x + 1;
      }
      repairMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      if (y + 1 < repairMaxY) {
        repairMaxY = y + 1;
      }
      for (repairY = repairMinY; repairY <= repairMaxY; repairY++) {
        for (repairX = repairMinX; repairX <= repairMaxX; repairX++) {
          repairDist = 0;
          if (x - repairX > 0) {
            repairDist += x - repairX;
          } else {
            repairDist += repairX - x;
          }
          if (y - repairY > 0) {
            repairDist += y - repairY;
          } else {
            repairDist += repairY - y;
          }
          if (repairDist < 1 || repairDist > 1) {
            continue;
          }
          if (!Tile_HasUnitToRepair(repairX, repairY)) {
            continue;
          }
          repairTarget =
              &MAP_STATE->units[MAP_STATE->mapTiles
                                    ->unitAt[MAP_TILE_INDEX(repairX, repairY)]];
          if (best == NULL || repairTarget->fuel < best->fuel) {
            best = repairTarget;
          }
        }
      }
      if (best == NULL) {
        return;
      }
      *target = Tile_GetDirection(x, y, best->x, best->y);
      *action = AI_ACTION_REPAIR;
      return;
    case UNIT_CRUISER:
    case UNIT_CARRIER:
      if (MAP_STATE->actingUnit->cargo[0] == 0) {
        return;
      }
      unit = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[0]];
      if (unit->fuel != Ov000_gUnitStats[unit->type].fuel) {
        return;
      }
      func_0209c1f8();
      unloadMinX = x - 1;
      if (unloadMinX < 0) {
        unloadStartX = 0;
      } else {
        unloadStartX = unloadMinX;
      }
      unloadMinY = y - 1;
      if (unloadMinY < 0) {
        unloadStartY = 0;
      } else {
        unloadStartY = unloadMinY;
      }
      unloadEndX = x + 1 < MAP_STATE->unk_1c4->mapWidth - 1
                       ? x + 1
                       : MAP_STATE->unk_1c4->mapWidth - 1;
      unloadEndY = y + 1 < MAP_STATE->unk_1c4->mapHeight - 1
                       ? y + 1
                       : MAP_STATE->unk_1c4->mapHeight - 1;
      for (unloadY = unloadStartY; unloadY <= unloadEndY; unloadY++) {
        for (unloadX = unloadStartX; unloadX <= unloadEndX; unloadX++) {
          unloadDist = 0;
          if (x - unloadX > 0) {
            unloadDist += x - unloadX;
          } else {
            unloadDist += unloadX - x;
          }
          if (y - unloadY > 0) {
            unloadDist += y - unloadY;
          } else {
            unloadDist += unloadY - y;
          }
          if (unloadDist < 1 || unloadDist > 1) {
            continue;
          }
          /* needed to match: read in place, not through chart (found by
           * tools/permuter.py) */
          if (((const s8(*)[32])gCoTable[0]
                   .blocks[0]
                   .movementCost[MOVE_CHART_DEFAULT])
                  [Ov000_gUnitStats[unit->type].moveType][MAP_TILE_TERRAIN(
                      MAP_STATE->mapTiles
                          ->tiles[MAP_TILE_INDEX(unloadX, unloadY)])] == -1) {
            continue;
          }
          if (MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(unloadX, unloadY)] !=
              0) {
            continue;
          }
          if (MAP_STATE->mapTiles->threatMax[MAP_TILE_INDEX(unloadX, unloadY)] +
                  MAP_STATE->mapTiles
                      ->threatSum[MAP_TILE_INDEX(unloadX, unloadY)] >
              UNLOAD_MAX_THREAT) {
            continue;
          }
          *action = AI_ACTION_UNLOAD;
          if (unloadX - x != 0) {
            *target = 3 - (unloadX - x);
          }
          if (unloadY - y != 0) {
            *target = unloadY - y + 2;
          }
          goto unloadSecond;
        }
      }
    unloadSecond:
      if (MAP_STATE->actingUnit->cargo[1] == 0) {
        return;
      }
      secondCargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[1]];
      if (secondCargo->fuel != Ov000_gUnitStats[secondCargo->type].fuel) {
        return;
      }
      /* needed to match: unloadStartX, unloadEndY, unloadMinX, unloadMinY
       * and unloadMaxY are set by a ternary or an if/else, not by an
       * assignment followed by an if. mwcc spills all five, and the form
       * decides which stack slot each one gets. */
      unloadMinX = unloadMinX < 0 ? 0 : unloadMinX;
      unloadMinY = unloadMinY < 0 ? 0 : unloadMinY;
      unloadMaxX = MAP_STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < unloadMaxX) {
        unloadMaxX = x + 1;
      }
      if (y + 1 >= MAP_STATE->unk_1c4->mapHeight - 1) {
        unloadMaxY = MAP_STATE->unk_1c4->mapHeight - 1;
      } else {
        unloadMaxY = y + 1;
      }
      /* needed to match: chart loads after the row check, once; the rows
         * count up in unloadMinY */
      if (unloadMinY > unloadMaxY) {
        return;
      }
      chart = (const s8(*)[32])gCoTable[0]
                  .blocks[0]
                  .movementCost[MOVE_CHART_DEFAULT];
      do {
        for (unload2X = unloadMinX; unload2X <= unloadMaxX; unload2X++) {
          unload2Dist = 0;
          if (x - unload2X > 0) {
            unload2Dist += x - unload2X;
          } else {
            unload2Dist += unload2X - x;
          }
          if (y - unloadMinY > 0) {
            unload2Dist += y - unloadMinY;
          } else {
            unload2Dist += unloadMinY - y;
          }
          if (unload2Dist < 1 || unload2Dist > 1) {
            continue;
          }
          if (MOVE_COST(secondCargo, unload2X, unloadMinY) == -1) {
            continue;
          }
          if (MAP_STATE->mapTiles
                  ->unitAt[MAP_TILE_INDEX(unload2X, unloadMinY)] != 0) {
            continue;
          }
          if (MAP_STATE->mapTiles
                      ->threatMax[MAP_TILE_INDEX(unload2X, unloadMinY)] +
                  MAP_STATE->mapTiles
                      ->threatSum[MAP_TILE_INDEX(unload2X, unloadMinY)] >
              UNLOAD_MAX_THREAT) {
            continue;
          }
          if (unload2X - x != 0) {
            *arg = 3 - (unload2X - x);
          }
          if (unloadMinY - y != 0) {
            *arg = unloadMinY - y + 2;
          }
          /* keep looking only while it matches the first cargo's direction */
          if (*target != *arg) {
            return;
          }
          *arg = 0;
        }
      } while (++unloadMinY <= unloadMaxY);
      return;
  }
}

/* Unit slot of the visible enemy unit next to (x, y) that AiAttack_Score
 * rates above 0 and highest for an attack from (x, y), 0 when there is none. */
int AiUnit_FindBestAdjacentTarget(int x, int y) {
  int maxY;
  int maxX;
  int minX;
  int cy;
  int bestSlot;
  int slot;
  int bestScore;
  int score;
  int cx;
  int dist;
  int minY;
  s16 pos[2];

  bestSlot = 0;
  bestScore = 0;
  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - 1;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (cy = minY; cy <= maxY; cy++) {
    for (cx = minX; cx <= maxX; cx++) {
      dist = 0;
      if (x - cx > 0) {
        dist += x - cx;
      } else {
        dist += cx - x;
      }
      if (y - cy > 0) {
        dist += y - cy;
      } else {
        dist += cy - y;
      }
      if (dist < 1 || dist > 1) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(cx, cy)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      if (!Map_IsTileVisible(cx, cy)) {
        continue;
      }
      pos[0] = x;
      pos[1] = y;
      score = AiAttack_Score(slot | AI_ATTACK_SAFE, pos);
      if (bestScore < score) {
        bestSlot = slot;
        bestScore = score;
      }
    }
  }
  return bestSlot;
}

/* For an indirect unit about to wait at (x, y): the unit slot of the best
 * enemy unit in its fire range from there (at or beyond the unit type's
 * minimum range, within its maximum range with CO bonuses), or 0 when none
 * is visible and safe to hit. Targets are scored by AiAttack_Score with
 * AI_ATTACK_SAFE. */
int AiUnit_PickIndirectTarget(int x, int y) {
  /* needed to match: declaration order and the u8 slots */
  s16 pos[2];
  u8 bestSlot;
  int minX;
  int maxX;
  int maxY;
  u8 slot;
  int bestScore;
  int minY;
  int cy;
  int cx;
  int score;
  int distance;

  bestSlot = 0;
  bestScore = 0;
  minX = (x - MAX_RANGE() < 0) ? 0 : x - MAX_RANGE();
  minY = (y - MAX_RANGE() < 0) ? 0 : y - MAX_RANGE();
  maxX = STATE_IDX_FIRST->unk_1c4->mapWidth - 1;
  if (x + MAX_RANGE() < maxX) {
    maxX = x + MAX_RANGE();
  }
  maxY = STATE_IDX_FIRST->unk_1c4->mapHeight - 1;
  if (y + MAX_RANGE() < maxY) {
    maxY = y + MAX_RANGE();
  }
  for (cy = minY; cy <= maxY; cy++) {
    for (cx = minX; cx <= maxX; cx++) {
      distance = 0;
      if (x - cx > 0) {
        distance += x - cx;
      } else {
        distance += cx - x;
      }
      if (y - cy > 0) {
        distance += y - cy;
      } else {
        distance += cy - y;
      }
      if (distance < Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange ||
          distance > MAX_RANGE()) {
        continue;
      }
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(cx, cy)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      if (!Map_IsTileVisible(cx, cy)) {
        continue;
      }
      pos[0] = x;
      pos[1] = y;
      score = AiAttack_Score(slot | AI_ATTACK_SAFE, pos);
      if (bestScore < score) {
        bestSlot = slot;
        bestScore = score;
      }
    }
  }
  return bestSlot;
}

/* AiUnit_AdjustAction calls this for action 2 (wait) at (x, y). If a
 * target can be hit from there, the action becomes an attack on its unit
 * slot. Direct units check any tile; indirect units only if they stay on
 * the tile they stand on. */
void AiUnit_WaitToAttack(int x, int y, u8* action, u8* target) {
  int slot;

  if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) == 0) {
    slot = AiUnit_FindBestAdjacentTarget(x, y);
    if (slot != 0) {
      *action = AI_ACTION_ATTACK_UNIT;
      *target = slot;
    }
  } else if (UnitType_GetAttackKind(MAP_STATE->actingUnit->type) == 1) {
    if (x == MAP_STATE->actingUnit->x && y == MAP_STATE->actingUnit->y) {
      slot = AiUnit_PickIndirectTarget(x, y);
      if (slot != 0) {
        *action = AI_ACTION_ATTACK_UNIT;
        *target = slot;
      }
    }
  }
}

/* Puts unit fromSlot back on its tile and makes toSlot the acting unit:
 * lifts it off its tile, records its start position and rolls a new
 * gAiTurnState->roll. */
void Ai_SwitchActingUnit(int fromSlot, int toSlot) {
  MapUnit* from = &MAP_STATE->units[fromSlot];
  MapUnit* to = &MAP_STATE->units[toSlot];

  from->flags &= ~MAP_UNIT_LOCKED;
  MAP_STATE->mapTiles
      ->unitAt[from->x + MAP_STATE->mapTiles->rowStart[from->y]] = fromSlot;
  MAP_STATE->actingUnit = to;
  MAP_STATE->actingUnitIndex = toSlot;
  to->flags |= MAP_UNIT_LOCKED;
  MAP_STATE->mapTiles->unitAt[to->x + MAP_STATE->mapTiles->rowStart[to->y]] = 0;
  MAP_STATE->actingUnitStartX = to->x;
  MAP_STATE->actingUnitStartY = to->y;
  gAiTurnState->roll = HwMod32(Rng_Next(), 100);
  gAiTurnState->unk_05 = Unit_GetUsableMovement(MAP_STATE->actingUnit);
}

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
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_FillUnitMoves(other);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  RangeMap_FillUnitMoves(MAP_STATE->actingUnit);
  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
      if (MAP_STATE->rangeMap[row][col] >= 0 &&
          MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(col, row)] < 0) {
        MAP_STATE->rangeMap[row][col] = -1;
      }
    }
  }
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0) {
        continue;
      }
      target =
          &MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == 0) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      score = AiAttack_Score(MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)] |
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
        maxX = MAP_STATE->unk_1c4->mapWidth - 1;
        if (target->x + 1 < maxX) {
          maxX = target->x + 1;
        }
        maxY = MAP_STATE->unk_1c4->mapHeight - 1;
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
            if (MAP_STATE->rangeMap[yy][xx] < 0) {
              continue;
            }
            neighbour = &MAP_STATE->units[MAP_STATE->mapTiles
                                              ->unitAt[MAP_TILE_INDEX(xx, yy)]];
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
      targetSlot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
    }
  }
  if (bestScore != 0) {
    AiUnit_PerformAction(bestX, bestY, 4, targetSlot, 0);
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_FillUnitMoves(other);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  AiUnit_MoveToSafestTile();
}

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
  pos[0] = MAP_STATE->actingUnit->x;
  pos[1] = MAP_STATE->actingUnit->y;
  if (MAP_STATE->actingUnit->x - GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      0) {
    minX = 0;
  } else {
    minX =
        MAP_STATE->actingUnit->x -
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  if (MAP_STATE->actingUnit->y - GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      0) {
    minY = 0;
  } else {
    minY =
        MAP_STATE->actingUnit->y -
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (MAP_STATE->actingUnit->x + GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      maxX) {
    maxX =
        MAP_STATE->actingUnit->x +
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (MAP_STATE->actingUnit->y + GetUnitMaxRange(MAP_STATE->currentPlayer,
                                                 MAP_STATE->actingUnit->type) <
      maxY) {
    maxY =
        MAP_STATE->actingUnit->y +
        GetUnitMaxRange(MAP_STATE->currentPlayer, MAP_STATE->actingUnit->type);
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      distance = 0;
      if (MAP_STATE->actingUnit->x - x > 0) {
        distance += MAP_STATE->actingUnit->x - x;
      } else {
        distance += x - MAP_STATE->actingUnit->x;
      }
      if (MAP_STATE->actingUnit->y - y > 0) {
        distance += MAP_STATE->actingUnit->y - y;
      } else {
        distance += y - MAP_STATE->actingUnit->y;
      }
      if (distance < Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange) {
        continue;
      }
      if (distance > GetUnitMaxRange(MAP_STATE->currentPlayer,
                                     MAP_STATE->actingUnit->type)) {
        continue;
      }
      target = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)]];
      if (target->type == UNIT_NONE) {
        continue;
      }
      if (MAP_STATE->players[(MAP_STATE->actingUnitIndex >> 6) + 1].team ==
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, target)].team) {
        continue;
      }
      if (Map_IsTileVisible(x, y) == 0) {
        continue;
      }
      score = AiAttack_Score(
          AI_ATTACK_HEAVY_HIT | AI_ATTACK_SAFE |
              MAP_TILES->unitAt[MAP_TILE_INDEX(target->x, target->y)],
          pos);
      if (score != 0 && score >= bestScore) {
        bestScore = score;
        bestSlot = MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)];
      }
    }
  }
  if (bestScore != 0) {
    AiUnit_PerformAction(MAP_STATE->actingUnit->x, MAP_STATE->actingUnit->y,
                         AI_ACTION_ATTACK_UNIT, bestSlot, 0);
  }
  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillUnitMoves(mover);
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  AiUnit_MoveToSafestTile();
}

/* AiUnit_AdjustAction calls this when the acting unit is about to do action 10
 * on (x, y), where another unit stands. Nothing happens if that unit has
 * waited or has AI order 7. Otherwise, if it is capturing or has more hp
 * than the acting unit, it becomes the acting unit for a moment: it goes
 * on capturing (action 3) and the attack planner for the original
 * unit's attack kind runs. If not, its slot is written at
 * gAiQueueCursor and the attack planner runs for it. */
void AiUnit_HandleUnitAtTarget(int x, int y) {
  MapUnit* other;
  MapUnit* acting;
  u8 slot;
  u8 actingIndex;
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];
  MapTiles* tiles = game->mapTiles;

  slot = tiles->unitAt[tiles->rowStart[y] + x];
  other = &game->units[slot];
  acting = game->actingUnit;
  actingIndex = game->actingUnitIndex;

  if (other->flags & MAP_UNIT_WAITED) {
    return;
  }
  if (other->aiOrder == 7) {
    return;
  }
  gAiTurnState->stepFlags |= AI_STEP_UNIT_AT_TARGET;
  if (other->captureProgress != 0 || acting->hp < other->hp) {
    Ai_SwitchActingUnit(actingIndex, slot);
    gAiQueueCursor--;
    if (other->captureProgress != 0) {
      AiUnit_PerformAction(other->x, other->y, 3, 0, 0);
    }
    /* 0: direct, 1: indirect */
    switch (UnitType_GetAttackKind(acting->type)) {
      case 0:
        AiUnit_PickDirectAttackInReachOf(acting);
        break;
      case 1:
        AiUnit_AttackFromPlaceThenMove(acting);
        break;
    }
    Ai_SwitchActingUnit(slot, actingIndex);
    gAiQueueCursor++;
  } else {
    *gAiQueueCursor = slot;
    gAiQueueCursor--;
    switch (UnitType_GetAttackKind(other->type)) {
      case 0:
        AiUnit_PickDirectAttackInReachOf(other);
        break;
      case 1:
        AiUnit_AttackFromPlaceThenMove(other);
        break;
    }
    gAiQueueCursor++;
  }
}
