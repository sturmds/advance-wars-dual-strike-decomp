#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"
#include "game/range_map.h"
#include "game/ai/ai_capture_site.h"
#include "game/ai/ai_move_search.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* the Unk_02080440_6088 entry of the property on a tile */
#define PROPERTY_AT(x, y) \
  (MAP_STATE->unk_6088[MAP_STATE->mapTiles->unk_1928[MAP_TILE_INDEX(x, y)]])

/* Callback the path search calls for each tile. */
extern int (*data_0229e174)(int a, int b, int c);

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);

/* TRUE when an enemy unit stands on tile (x, y), or the move search of
 * func_020a5cc8 for unitType and maxCost reaches one. */
BOOL Ai_HasEnemyWithinMoves(int x, int y, int unitType, int maxCost) {
  data_0229e174 = Tile_HasEnemyUnit;
  if (data_0229e174(x, y, unitType) == 1) {
    return TRUE;
  }
  return func_020a5cc8(x, y, unitType, maxCost);
}

/* Runs the path search with the Ai_CanDamageEnemyAtTile callback installed. */
int func_020a50d4(int x, int y, int unitType, int movement) {
  data_0229e174 = Ai_CanDamageEnemyAtTile;
  return MoveSearch_FindTile(x, y, unitType, movement);
}

int func_020a507c(int x, int y, int range) {
  data_0229e174 = Ai_IsFreeCaptureSiteTile;
  if (data_0229e174(x, y, 0) == 1) {
    return 1;
  }
  return MoveSearch_FindTileByMechSteps(x, y, 0, range);
}

/* Direction from the first tile to the second, comparing x before y:
 * 0 same tile, 1 up, 2 right, 3 down, 4 left. */
int Tile_GetDirection(int fromX, int fromY, int toX, int toY) {
  if (fromX > toX) {
    return 4;
  }
  if (fromX < toX) {
    return 2;
  }
  if (fromY > toY) {
    return 1;
  }
  if (fromY < toY) {
    return 3;
  }
  return 0;
}

/* TRUE when a unit of the current player is already capturing the tile (x, y),
 * or an Infantry or Mech whose AI target is the tile can reach it: Infantry
 * within 3 moves, Mech also within 2 tiles of it. Uses MapTiles.rangeScratch
 * for the Infantry move costs and restores the default range map. */
BOOL Ai_IsCaptureSiteTaken(int x, int y) {
  int maxX;
  int maxY;
  int cy;
  int minX;
  int cx;
  int distance;
  int slot;
  MapUnit* unit;

  RangeMap_SetBuffer(MAP_TILES->rangeScratch);
  RangeMap_FillMoves(x, y, UNIT_INFANTRY, 3, 1);
  slot = MAP_TILES->unitAt[MAP_TILE_INDEX(x, y)];
  if (slot != 0 && Unit_IsCurrentPlayers(slot) == TRUE &&
      MAP_STATE->units[slot].captureProgress != 0) {
    RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
    return TRUE;
  }
  minX = x - 3;
  if (minX < 0) {
    minX = 0;
  }
  cy = y - 3;
  if (cy < 0) {
    cy = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + 3 < maxX) {
    maxX = x + 3;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + 3 < maxY) {
    maxY = y + 3;
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
      if (distance < 0 || distance > 3) {
        continue;
      }
      if (MAP_STATE->rangeMap[cy][cx] < 0) {
        continue;
      }
      unit = &MAP_STATE->units[MAP_TILES->unitAt[MAP_TILE_INDEX(cx, cy)]];
      if (unit->type == 0 || unit->type > UNIT_MECH) {
        continue;
      }
      if (unit->aiTargetX != x || unit->aiTargetY != y) {
        continue;
      }
      if (unit->type == UNIT_MECH &&
          Map_GetDistance(unit->x, unit->y, x, y) > 2) {
        continue;
      }
      RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
      return TRUE;
    }
  }
  RangeMap_SetBuffer(MAP_TILES->rangeMapTiles);
  return FALSE;
}

/* Picks the property within distance tiles of (x, y) (Manhattan) that is a
 * silo or capturable tile and writes it to property. A tile that another
 * unit has claimed (unk_03) is skipped when Ai_IsCaptureSiteTaken says so. Terrain
 * decides the pick: HQ 5, base 4, silo 3, airport 2, port and comm tower 1,
 * anything else 0. Ties go to the later tile. FALSE: nothing found. */
BOOL Ai_FindCaptureSiteNear(int x, int y, int distance, s16* property) {
  /* needed to match: declaration order sets registers and stack slots */
  int maxY;
  int minX;
  int bestX;
  int bestY;
  int cy;
  int bestValue;
  int cx;
  int maxX;
  int minY;
  int dist;
  int value;

  bestX = -1;
  bestY = -1;
  bestValue = -1;
  minX = x - distance;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - distance;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + distance < maxX) {
    maxX = x + distance;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
  if (y + distance < maxY) {
    maxY = y + distance;
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
      if (dist < 0 || dist > distance) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(cx, cy)) {
        continue;
      }
      if (PROPERTY_AT(cx, cy).claimCount != 0 &&
          Ai_IsCaptureSiteTaken(cx, cy)) {
        continue;
      }
      switch (MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(cx, cy)])) {
        case TERRAIN_HQ:
          value = 5;
          break;
        case TERRAIN_BASE:
          value = 4;
          break;
        case TERRAIN_SILO:
          value = 3;
          break;
        case TERRAIN_AIRPORT:
          value = 2;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          value = 1;
          break;
        default:
          value = 0;
          break;
      }
      if (value >= bestValue) {
        bestValue = value;
        bestX = cx;
        bestY = cy;
      }
    }
  }
  if (bestX == -1) {
    return FALSE;
  }
  property[0] = bestX;
  property[1] = bestY;
  return TRUE;
}

/* Fills rangeMap with the moves of the unit's type from its tile, with a
 * movement of 110 (far more than any unit has), then returns TRUE if any
 * tile it reaches is a silo or property Map_IsSiloOrCapturableTile accepts. */
BOOL AiUnit_CanReachCaptureSite(MapUnit* unit) {
  int x;
  int y;

  RangeMap_FillMoves(unit->x, unit->y, unit->type, 110, 0);
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] >= 0 &&
          Map_IsSiloOrCapturableTile(x, y) == TRUE) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* Clears the acting unit's AI target when it is a foot unit (infantry or
 * mech). Called before the AI queues an attack. */
void Ai_ClearActingUnitTarget(void) {
  if (data_027c027c->unk_48[data_027c0284]->actingUnit->type > UNIT_MECH) {
    return;
  }
  data_027c027c->unk_48[data_027c0284]->actingUnit->aiTargetX =
      MAP_UNIT_NO_TARGET;
  data_027c027c->unk_48[data_027c0284]->actingUnit->aiTargetY =
      MAP_UNIT_NO_TARGET;
}
