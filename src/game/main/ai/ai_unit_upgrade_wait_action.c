#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/co_data.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Map_IsCapturableByActingUnit(s16 x, s16 y);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
int Unit_GetCoMaxRange(MapUnit* unit); /* max range with the CO's range bonus */
int Player_GetUnitMoveRange(
    int player, int unitType); /* movement plus the CO's move bonus */
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(s8 mark);
int UnitType_BestBaseDamage(int unitType, int targetType);
BOOL Tile_HasUnitToSupply(
    s16 x,
    s16 y); /* another unit of the current player on (x, y) short of ammo or fuel */
BOOL Tile_HasUnitToRepair(
    s16 x, s16 y); /* the same, or below 10 shown hp; never Oozium */
int Tile_GetDirection(
    int x, int y, int toX,
    int toY); /* direction from (x, y) to (toX, toY): 1 up, 2 right, 3 down, 4 left, 0 same */
void func_0209c1f8(void); /* rebuilds the AI threat maps */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define MOVE_RANGE(unit) \
  Player_GetUnitMoveRange(MAP_UNIT_OWNER(STATE, unit), (unit)->type)
#define MOVE_COST(unit, x, y)                    \
  chart[Ov000_gUnitStats[(unit)->type].moveType] \
       [MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE_INDEX(x, y)])]
#define PLAYER_OF_SLOT(slot) (STATE->players[((slot) >> 6) + 1])

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
  switch (STATE->actingUnit->type) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      if (Map_IsCapturableByActingUnit(x, y) == TRUE) {
        *action = AI_ACTION_CAPTURE;
      }
      return;
    case UNIT_SUB:
      if (Unit_CanRepairAt(STATE->actingUnit, x, y) == TRUE ||
          Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == TRUE) {
        if (!(STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
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
      subMaxX = STATE->unk_1c4->mapWidth - 1;
      if (x + 8 < subMaxX) {
        subMaxX = x + 8;
      }
      subMaxY = STATE->unk_1c4->mapHeight - 1;
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
          subSlot = STATE->mapTiles->unitAt[TILE_INDEX(subX, subY)];
          if (subSlot == 0) {
            continue;
          }
          if (PLAYER_OF_SLOT(STATE->actingUnitIndex).team ==
              PLAYER_OF_SLOT(subSlot).team) {
            continue;
          }
          unit = &STATE->units[subSlot];
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
              if (STATE->rangeMap[y][x] < 0) {
                continue;
              }
            }
          }
          if (UnitType_BestBaseDamage(unit->type, UNIT_SUB) >= 10) {
            count++;
          }
        }
      }
      if (count != 0 && !(STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_HIDE;
        return;
      }
      if (count == 0 && (STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_APPEAR;
      }
      return;
    case UNIT_STEALTH:
      if (Unit_CanRepairAt(STATE->actingUnit, x, y) == TRUE ||
          Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y) == TRUE) {
        if (!(STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
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
      stealthMaxX = STATE->unk_1c4->mapWidth - 1;
      if (x + 10 < stealthMaxX) {
        stealthMaxX = x + 10;
      }
      stealthMaxY = STATE->unk_1c4->mapHeight - 1;
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
          stealthSlot = STATE->mapTiles->unitAt[TILE_INDEX(stealthX, stealthY)];
          if (stealthSlot == 0) {
            continue;
          }
          if (PLAYER_OF_SLOT(STATE->actingUnitIndex).team ==
              PLAYER_OF_SLOT(stealthSlot).team) {
            continue;
          }
          unit = &STATE->units[stealthSlot];
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
      if (count != 0 && !(STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_HIDE;
        return;
      }
      if (count == 0 && (STATE->actingUnit->flags & MAP_UNIT_DIVED)) {
        *action = AI_ACTION_APPEAR;
      }
      return;
    case UNIT_APC:
      if (x == STATE->actingUnit->x && y == STATE->actingUnit->y) {
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
      supplyMaxX = STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < supplyMaxX) {
        supplyMaxX = x + 1;
      }
      supplyMaxY = STATE->unk_1c4->mapHeight - 1;
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
      if (x == STATE->actingUnit->x && y == STATE->actingUnit->y) {
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
      repairMaxX = STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < repairMaxX) {
        repairMaxX = x + 1;
      }
      repairMaxY = STATE->unk_1c4->mapHeight - 1;
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
              &STATE->units[STATE->mapTiles
                                ->unitAt[TILE_INDEX(repairX, repairY)]];
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
      if (STATE->actingUnit->cargo[0] == 0) {
        return;
      }
      unit = &STATE->units[STATE->actingUnit->cargo[0]];
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
      unloadEndX = x + 1 < STATE->unk_1c4->mapWidth - 1
                       ? x + 1
                       : STATE->unk_1c4->mapWidth - 1;
      unloadEndY = y + 1 < STATE->unk_1c4->mapHeight - 1
                       ? y + 1
                       : STATE->unk_1c4->mapHeight - 1;
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
                      STATE->mapTiles->tiles[TILE_INDEX(unloadX, unloadY)])] ==
              -1) {
            continue;
          }
          if (STATE->mapTiles->unitAt[TILE_INDEX(unloadX, unloadY)] != 0) {
            continue;
          }
          if (STATE->mapTiles->threatMax[TILE_INDEX(unloadX, unloadY)] +
                  STATE->mapTiles->threatSum[TILE_INDEX(unloadX, unloadY)] >
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
      if (STATE->actingUnit->cargo[1] == 0) {
        return;
      }
      secondCargo = &STATE->units[STATE->actingUnit->cargo[1]];
      if (secondCargo->fuel != Ov000_gUnitStats[secondCargo->type].fuel) {
        return;
      }
      /* needed to match: unloadStartX, unloadEndY, unloadMinX, unloadMinY
       * and unloadMaxY are set by a ternary or an if/else, not by an
       * assignment followed by an if. mwcc spills all five, and the form
       * decides which stack slot each one gets. */
      unloadMinX = unloadMinX < 0 ? 0 : unloadMinX;
      unloadMinY = unloadMinY < 0 ? 0 : unloadMinY;
      unloadMaxX = STATE->unk_1c4->mapWidth - 1;
      if (x + 1 < unloadMaxX) {
        unloadMaxX = x + 1;
      }
      if (y + 1 >= STATE->unk_1c4->mapHeight - 1) {
        unloadMaxY = STATE->unk_1c4->mapHeight - 1;
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
          if (STATE->mapTiles->unitAt[TILE_INDEX(unload2X, unloadMinY)] != 0) {
            continue;
          }
          if (STATE->mapTiles->threatMax[TILE_INDEX(unload2X, unloadMinY)] +
                  STATE->mapTiles->threatSum[TILE_INDEX(unload2X, unloadMinY)] >
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
