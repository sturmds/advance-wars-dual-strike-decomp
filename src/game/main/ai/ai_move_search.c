#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/unit_stats.h"
#include "game/map_unit.h"
#include "game/math.h"
#include "game/ai/ai_move_search.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define FROM_START 1
#define DIR_UP 2
#define DIR_DOWN 3
#define DIR_LEFT 4
#define DIR_RIGHT 5
/* steps from the current node; TRUE when the search callback accepts the tile */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
/* data_027c027c->unk_48[data_027c0284]. Needed to match: the original
 * loads the index before the base; a volatile read of the index is the
 * only form found that keeps that order. */
#define READ_STATE(dst)                          \
  {                                              \
    int index_ = *(volatile int*)&data_027c0284; \
    dst = data_027c027c->unk_48[index_];         \
  }
#define NOT_REACHABLE 111
/* Needed to match: at these two points the original reads data_027c0284 and
 * then data_027c027c again instead of reusing the state pointer it already
 * holds. Volatile reads, index first, are the only form found that does this. */
#define RELOAD_STATE(dst)                                             \
  {                                                                   \
    int index_ = *(volatile int*)&data_027c0284;                      \
    dst = (*(Unk_02080440* volatile*)&data_027c027c)->unk_48[index_]; \
  }
/* Daily fuel use added on top of Player_GetAirFuelChange's value, per turn spent
 * travelling. */
#define FUEL_PER_DAY_STEALTH 8
#define FUEL_PER_DAY_AIR_SUB 5
#define FUEL_PER_DAY_OTHER 1
#define TILE(tiles, x, y) ((x) + (tiles)->rowStart[y])

/* Needed to match: enum results put moveq before movne. */
enum { SAME_TEAM, OTHER_TEAM };

extern s8 data_027c074c;
extern int data_027c0750;
extern MoveSearchNode* data_027c0754;
extern MoveSearch* data_027c0758;
extern MoveSearchNode* data_027c075c;
extern BOOL (*data_0229e174)(int x, int y, int unitType);

int Player_GetUnitMoveRange(int player, int unitType);
BOOL UnitType_CanEnterTerrain(int unitType, u8 terrain);
void MoveSearch_LoadMoveCosts(int unitType);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int UnitType_BestBaseDamage(int unitType, int targetType);
int Player_GetFuelRate(int player);
int Player_GetAirFuelChange(int player, int unitType);

/* The step of RangeMap_FillMovesOverTurns: moves from the node being
 * expanded one tile in direction. If the cost of the move is within
 * maxCost and beats what rangeMap holds for the tile, records it there,
 * records the turn count in MapTiles.rangeScratch, and queues the tile. */
void MoveSearch_TryStepOverTurns(int direction) {
  s8* rangeTile;
  MapTiles* tiles;
  int cost;
  int turns;
  int turnEnd;
  Unk_02080440_1c4* map;
  u8 x;
  unsigned long row;
  Unk_02080440_48* state;
  u8 y;
  u8 terrainCost;

  data_027c0758->x = data_027c0754->x;
  data_027c0758->y = data_027c0754->y;
  switch (direction) {
    case DIR_UP:
      data_027c0758->y--;
      break;
    case DIR_DOWN:
      data_027c0758->y++;
      break;
    case DIR_LEFT:
      data_027c0758->x--;
      break;
    case DIR_RIGHT:
      data_027c0758->x++;
      break;
  }
  if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
    return;
  }
  READ_STATE(state);
  map = state->unk_1c4;
  x = data_027c0758->x;
  /* x and y are unsigned: a step off the top or left edge wraps to 255 */
  if (x >= map->mapWidth) {
    return;
  }
  y = data_027c0758->y;
  if (y >= map->mapHeight) {
    return;
  }
  tiles = state->mapTiles;
  /* Needed to match: an unsigned long copy of y, set here (u32 or a
     * cast does not match) */
  row = y;
  terrainCost =
      data_027c0758
          ->moveCost[MAP_TILE_TERRAIN(tiles->tiles[TILE(tiles, x, row)])];
  cost = data_027c0754->cost + terrainCost;
  if (cost > data_027c0758->maxCost) {
    return;
  }
  turns = tiles->rangeScratch[TILE(tiles, data_027c0754->x, data_027c0754->y)];
  /* a move that does not fit in the current turn starts the next one,
     * and the movement left over in this turn is lost */
  turnEnd = data_027c0758->moveRange * turns;
  if (turnEnd < cost) {
    cost = turnEnd + terrainCost;
    turns++;
  }
  rangeTile = &state->rangeMap[row][x];
  if (*rangeTile <= cost && *rangeTile >= 0) {
    return;
  }
  *rangeTile = cost;
  MAP_STATE->mapTiles->rangeScratch[TILE(MAP_STATE->mapTiles, data_027c0758->x,
                                         data_027c0758->y)] = turns;
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = cost;
  data_027c075c++;
  data_027c0750++;
}

/* Fills rangeMap with the move cost from (x, y) for a unit type, as
 * RangeMap_FillMoves does, but a path may go on past the unit's movement
 * for several turns: MapTiles.rangeScratch gets the number of turns each
 * tile takes and rangeMap the cost within its last turn. maxCost limits
 * the search. Both maps hold -1 for tiles not reached. */
void RangeMap_FillMovesOverTurns(int x, int y, int unitType, int maxCost) {
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
  RangeMap_Fill(-1);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(-1);
  data_027c0750 = 0;
  data_027c074c = 0;
  data_027c075c = data_027c0758->lists[0];
  data_027c0754 = data_027c0758->lists[1];
  data_027c0758->maxCost = maxCost;
  data_027c0758->moveRange =
      Player_GetUnitMoveRange(MAP_STATE->currentPlayer, unitType);
  MoveSearch_LoadMoveCosts(unitType);
  data_027c075c->x = x;
  data_027c075c->y = y;
  data_027c075c->from = FROM_START;
  data_027c075c->cost = 0;
  MAP_STATE->rangeMap[y][x] = 0;
  MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)] = 0;
  data_027c075c++;
  data_027c075c->from = 0;
  for (;;) {
    /* swap the lists: expand what was found, collect into the other */
    if (data_027c074c == 0) {
      data_027c074c = 1;
      data_027c075c = data_027c0758->lists[1];
      data_027c0754 = data_027c0758->lists[0];
      data_027c0750 = 0;
    } else {
      data_027c074c = 0;
      data_027c075c = data_027c0758->lists[0];
      data_027c0754 = data_027c0758->lists[1];
      data_027c0750 = 0;
    }
    if (data_027c0754->from == 0) {
      break;
    }
    while (data_027c0754->from != 0) {
      switch (data_027c0754->from) {
        case FROM_START:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_UP:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_DOWN:
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
        case DIR_LEFT:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_LEFT);
          break;
        case DIR_RIGHT:
          MoveSearch_TryStepOverTurns(DIR_UP);
          MoveSearch_TryStepOverTurns(DIR_DOWN);
          MoveSearch_TryStepOverTurns(DIR_RIGHT);
          break;
      }
      data_027c075c->from = 0;
      data_027c0754++;
    }
  }
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
}

/* TRUE when the acting unit can reach (x, y) with fuel left over for the
 * extra days. rangeMap holds the move cost there; the days are the whole
 * turns beyond the first at movesPerTurn. Ground units and unreached tiles
 * (cost 0) always pass. */
BOOL AiUnit_HasFuelForTile(s16 x, s16 y, int movesPerTurn) {
  int days;
  int unitType;
  Unk_02080440_48* state;

  if (!(Ov000_gUnitStats[MAP_STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_AIR | MOVE_GROUP_SEA))) {
    return TRUE;
  }
  if (MAP_STATE->rangeMap[y][x] == 0) {
    return TRUE;
  }
  days = HwDiv32(MAP_STATE->rangeMap[y][x] - 1, movesPerTurn);
  RELOAD_STATE(state);
  unitType = state->actingUnit->type;
  if (unitType == UNIT_STEALTH) {
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_STEALTH)) {
      return FALSE;
    }
  } else if ((Ov000_gUnitStats[unitType].moveGroup & MOVE_GROUP_AIR) ||
             unitType == UNIT_SUB) {
    RELOAD_STATE(state);
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_AIR_SUB)) {
      return FALSE;
    }
  } else {
    if (state->actingUnit->fuel -
            Player_GetFuelRate(state->currentPlayer) * state->rangeMap[y][x] <
        days * (Player_GetAirFuelChange(state->currentPlayer, unitType) +
                FUEL_PER_DAY_OTHER)) {
      return FALSE;
    }
  }
  return TRUE;
}

/* Lowest rangeMap value among the tiles next to (x, y), 111 when none is
 * reachable. Tiles outside the map are skipped. */
int AiTile_GetLowestAdjacentReach(int x, int y) {
  /* needed to match: declaration order sets registers and stack slots */
  int maxY;
  int maxX;
  int cy;
  int minX;
  int best;
  int cx;
  int distance;
  int reach;

  minX = x - 1;
  if (minX < 0) {
    minX = 0;
  }
  cy = y - 1;
  if (cy < 0) {
    cy = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  best = NOT_REACHABLE;
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 1 < maxY) {
    maxY = y + 1;
  }
  for (; cy <= maxY; cy++) {
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
      if (distance < 1 || distance > 1) {
        continue;
      }
      reach = MAP_STATE->rangeMap[cy][cx];
      if (reach < 0) {
        continue;
      }
      if (reach < best) {
        best = reach;
      }
    }
  }
  return best;
}

/* TRUE when tile (x, y) holds a unit whose owner is on another team than
 * the current player. */
int Tile_HasEnemyUnit(int x, int y, int arg2) {
  u8 slot = MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];

  if (slot == 0) {
    return FALSE;
  }
  return MAP_STATE->players[MAP_STATE->currentPlayer].team ==
                 MAP_STATE->players[(slot >> 6) + 1].team
             ? SAME_TEAM
             : OTHER_TEAM;
}

/* Path search callback: TRUE when an enemy unit stands on tile (x, y) and
 * unitType can damage it (chart damage above 1). */
int Ai_CanDamageEnemyAtTile(int x, int y, int unitType) {
  if (!Tile_HasEnemyUnit(x, y, 0)) {
    return FALSE;
  }
  return UnitType_BestBaseDamage(
             unitType,
             MAP_STATE
                 ->units[MAP_STATE->mapTiles
                             ->unitAt[x + MAP_STATE->mapTiles->rowStart[y]]]
                 .type) > 1;
}

/* Path search callback: TRUE for a silo or capturable tile that is empty,
 * or holds one of the current player's units that is not Infantry or Mech. */
int Ai_IsFreeCaptureSiteTile(int x, int y, int arg2) {
  int slot;

  if (!Map_IsSiloOrCapturableTile(x, y)) {
    return FALSE;
  }
  slot = MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
  if (slot == 0) {
    return TRUE;
  }
  if (!Unit_IsCurrentPlayers(slot)) {
    return FALSE;
  }
  if (MAP_STATE->units[slot].type > UNIT_MECH) {
    return TRUE;
  }
  return FALSE;
}

/* MoveSearch_TryStep with a stricter limit: the cost of the path including
 * the new tile must stay within maxCost before the callback sees the tile.
 * TRUE when the callback accepts it; otherwise the tile is queued with
 * that cost. */
BOOL MoveSearch_TryStepInCost(int direction) {
  Unk_02080440_48* state;
  Unk_02080440_1c4* map;
  MapTiles* tiles;
  int cost;

  data_027c0758->x = data_027c0754->x;
  data_027c0758->y = data_027c0754->y;
  switch (direction) {
    case DIR_UP:
      data_027c0758->y--;
      break;
    case DIR_DOWN:
      data_027c0758->y++;
      break;
    case DIR_LEFT:
      data_027c0758->x--;
      break;
    case DIR_RIGHT:
      data_027c0758->x++;
      break;
  }
  if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
    return FALSE;
  }
  READ_STATE(state);
  map = state->unk_1c4;
  /* x and y are unsigned: a step off the top or left edge wraps to 255 */
  if (data_027c0758->x >= map->mapWidth || data_027c0758->y >= map->mapHeight) {
    return FALSE;
  }
  tiles = state->mapTiles;
  cost =
      data_027c0754->cost +
      data_027c0758->moveCost[MAP_TILE_TERRAIN(
          tiles->tiles[data_027c0758->x + tiles->rowStart[data_027c0758->y]])];
  if (cost > data_027c0758->maxCost) {
    return FALSE;
  }
  if (data_0229e174(data_027c0758->x, data_027c0758->y,
                    data_027c0758->unitType) == TRUE) {
    return TRUE;
  }
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = cost;
  data_027c075c++;
  data_027c0750++;
  return FALSE;
}

/* MoveSearch_FindTile with MoveSearch_TryStepInCost as the step: same breadth-first
 * search, a different step test. */
BOOL func_020a5cc8(int x, int y, int unitType, int maxCost) {
  data_027c0750 = 0;
  data_027c074c = 0;
  data_027c075c = data_027c0758->lists[0];
  data_027c0754 = data_027c0758->lists[1];
  data_027c0758->maxCost = maxCost;
  data_027c0758->moveRange =
      Player_GetUnitMoveRange(MAP_STATE->currentPlayer, unitType);
  data_027c0758->unitType = unitType;
  MoveSearch_LoadMoveCosts(unitType);
  data_027c075c->x = x;
  data_027c075c->y = y;
  data_027c075c->from = FROM_START;
  data_027c075c->cost = 0;
  data_027c075c++;
  data_027c075c->from = 0;
  for (;;) {
    /* swap the lists: expand what was found, collect into the other */
    if (data_027c074c == 0) {
      data_027c074c = 1;
      data_027c075c = data_027c0758->lists[1];
      data_027c0754 = data_027c0758->lists[0];
      data_027c0750 = 0;
    } else {
      data_027c074c = 0;
      data_027c075c = data_027c0758->lists[0];
      data_027c0754 = data_027c0758->lists[1];
      data_027c0750 = 0;
    }
    if (data_027c0754->from == 0) {
      break;
    }
    while (data_027c0754->from != 0) {
      switch (data_027c0754->from) {
        case FROM_START:
          if (MoveSearch_TryStepInCost(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_UP:
          if (MoveSearch_TryStepInCost(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_DOWN:
          if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_LEFT:
          if (MoveSearch_TryStepInCost(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_LEFT) == TRUE)
            goto found;
          break;
        case DIR_RIGHT:
          if (MoveSearch_TryStepInCost(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStepInCost(DIR_RIGHT) == TRUE)
            goto found;
          break;
      }
      data_027c075c->from = 0;
      data_027c0754++;
    }
  }
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return FALSE;

found:
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return TRUE;
}

/* One step of MoveSearch_FindTile: moves from the node being expanded one
 * tile in direction and, if the path so far is within maxCost and the tile
 * is on the map, asks the callback about it. TRUE when the callback
 * accepts the tile; otherwise the tile is queued with the path cost so far
 * plus its terrain's move cost (capped at 255). */
BOOL MoveSearch_TryStep(int direction) {
  Unk_02080440_48* state;
  Unk_02080440_1c4* map;
  MapTiles* tiles;
  int cost;

  if (data_027c0754->cost > data_027c0758->maxCost) {
    return FALSE;
  }
  data_027c0758->x = data_027c0754->x;
  data_027c0758->y = data_027c0754->y;
  switch (direction) {
    case DIR_UP:
      data_027c0758->y--;
      break;
    case DIR_DOWN:
      data_027c0758->y++;
      break;
    case DIR_LEFT:
      data_027c0758->x--;
      break;
    case DIR_RIGHT:
      data_027c0758->x++;
      break;
  }
  if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
    return FALSE;
  }
  READ_STATE(state);
  map = state->unk_1c4;
  /* x and y are unsigned: a step off the top or left edge wraps to 255 */
  if (data_027c0758->x >= map->mapWidth || data_027c0758->y >= map->mapHeight) {
    return FALSE;
  }
  tiles = state->mapTiles;
  cost =
      data_027c0754->cost +
      data_027c0758->moveCost[MAP_TILE_TERRAIN(
          tiles->tiles[data_027c0758->x + tiles->rowStart[data_027c0758->y]])];
  if (cost > 255) {
    cost = 255;
  }
  if (data_0229e174(data_027c0758->x, data_027c0758->y,
                    data_027c0758->unitType) == TRUE) {
    return TRUE;
  }
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = cost;
  data_027c075c++;
  data_027c0750++;
  return FALSE;
}

/* Breadth-first move search from (x, y) for a unit type, up to maxCost.
 * MoveSearch_TryStep hands each tile reached to the callback in
 * data_0229e174. Returns TRUE as soon as the callback accepts a tile,
 * FALSE once every reachable tile was tried. */
BOOL MoveSearch_FindTile(int x, int y, int unitType, int maxCost) {
  data_027c0750 = 0;
  data_027c074c = 0;
  data_027c075c = data_027c0758->lists[0];
  data_027c0754 = data_027c0758->lists[1];
  data_027c0758->maxCost = maxCost;
  data_027c0758->moveRange =
      Player_GetUnitMoveRange(MAP_STATE->currentPlayer, unitType);
  data_027c0758->unitType = unitType;
  MoveSearch_LoadMoveCosts(unitType);
  data_027c075c->x = x;
  data_027c075c->y = y;
  data_027c075c->from = FROM_START;
  data_027c075c->cost = 0;
  data_027c075c++;
  data_027c075c->from = 0;
  for (;;) {
    /* swap the lists: expand what was found, collect into the other */
    if (data_027c074c == 0) {
      data_027c074c = 1;
      data_027c075c = data_027c0758->lists[1];
      data_027c0754 = data_027c0758->lists[0];
      data_027c0750 = 0;
    } else {
      data_027c074c = 0;
      data_027c075c = data_027c0758->lists[0];
      data_027c0754 = data_027c0758->lists[1];
      data_027c0750 = 0;
    }
    if (data_027c0754->from == 0) {
      break;
    }
    while (data_027c0754->from != 0) {
      switch (data_027c0754->from) {
        case FROM_START:
          if (MoveSearch_TryStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_UP:
          if (MoveSearch_TryStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_DOWN:
          if (MoveSearch_TryStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_LEFT:
          if (MoveSearch_TryStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_LEFT) == TRUE)
            goto found;
          break;
        case DIR_RIGHT:
          if (MoveSearch_TryStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
      }
      data_027c075c->from = 0;
      data_027c0754++;
    }
  }
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return FALSE;

found:
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return TRUE;
}

/* Steps from the node being expanded (data_027c0754) in `direction`. A tile
 * on the map that a Mech can enter, within maxCost steps, goes to the
 * callback in data_0229e174; if the callback does not accept it, it is added
 * to the list being collected (data_027c075c). TRUE when the callback accepts
 * the tile. */
BOOL MoveSearch_TryMechStep(int direction) {
  int steps;

  data_027c0758->x = data_027c0754->x;
  data_027c0758->y = data_027c0754->y;
  switch (direction) {
    case DIR_UP:
      data_027c0758->y--;
      break;
    case DIR_DOWN:
      data_027c0758->y++;
      break;
    case DIR_LEFT:
      data_027c0758->x--;
      break;
    case DIR_RIGHT:
      data_027c0758->x++;
      break;
  }
  if (data_027c0750 >= MOVE_SEARCH_LIST_SIZE - 1) {
    return FALSE;
  }
  if (data_027c0758->x >= MAP_STATE->unk_1c4->mapWidth ||
      data_027c0758->y >= MAP_STATE->unk_1c4->mapHeight) {
    return FALSE;
  }
  if (!UnitType_CanEnterTerrain(
          UNIT_MECH, MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(
                         data_027c0758->x, data_027c0758->y)]))) {
    return FALSE;
  }
  steps = data_027c0754->cost + 1;
  if (steps > data_027c0758->maxCost) {
    return FALSE;
  }
  if (data_0229e174(data_027c0758->x, data_027c0758->y,
                    data_027c0758->unitType) == TRUE) {
    return TRUE;
  }
  data_027c075c->x = data_027c0758->x;
  data_027c075c->y = data_027c0758->y;
  data_027c075c->from = direction;
  data_027c075c->cost = steps;
  data_027c075c++;
  data_027c0750++;
  return FALSE;
}

/* MoveSearch_FindTile without move costs: a breadth-first search from
 * (x, y) over tiles a Mech can enter, up to maxCost steps. Returns TRUE as
 * soon as the callback in data_0229e174 accepts a tile, FALSE once every
 * tile in reach was tried. The only caller passes unit type 0. */
BOOL MoveSearch_FindTileByMechSteps(int x, int y, int unitType, int maxCost) {
  data_027c0750 = 0;
  data_027c074c = 0;
  data_027c075c = data_027c0758->lists[0];
  data_027c0754 = data_027c0758->lists[1];
  data_027c0758->maxCost = maxCost;
  data_027c0758->moveRange =
      Player_GetUnitMoveRange(MAP_STATE->currentPlayer, unitType);
  data_027c0758->unitType = unitType;
  data_027c075c->x = x;
  data_027c075c->y = y;
  data_027c075c->from = FROM_START;
  data_027c075c->cost = 0;
  data_027c075c++;
  data_027c075c->from = 0;
  for (;;) {
    /* swap the lists: expand what was found, collect into the other */
    if (data_027c074c == 0) {
      data_027c074c = 1;
      data_027c075c = data_027c0758->lists[1];
      data_027c0754 = data_027c0758->lists[0];
      data_027c0750 = 0;
    } else {
      data_027c074c = 0;
      data_027c075c = data_027c0758->lists[0];
      data_027c0754 = data_027c0758->lists[1];
      data_027c0750 = 0;
    }
    if (data_027c0754->from == 0) {
      break;
    }
    while (data_027c0754->from != 0) {
      switch (data_027c0754->from) {
        case FROM_START:
          if (MoveSearch_TryMechStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_UP:
          if (MoveSearch_TryMechStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_DOWN:
          if (MoveSearch_TryMechStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_LEFT) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
        case DIR_LEFT:
          if (MoveSearch_TryMechStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_LEFT) == TRUE)
            goto found;
          break;
        case DIR_RIGHT:
          if (MoveSearch_TryMechStep(DIR_UP) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_DOWN) == TRUE)
            goto found;
          if (MoveSearch_TryMechStep(DIR_RIGHT) == TRUE)
            goto found;
          break;
      }
      data_027c075c->from = 0;
      data_027c0754++;
    }
  }
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return FALSE;

found:
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
  return TRUE;
}
