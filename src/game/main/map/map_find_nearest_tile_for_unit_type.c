#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

/* TRUE when (x, y) is on the map, is free (with arg3 0: no unit there) and
 * unitType can move onto its terrain. */
BOOL Tile_IsFreeForUnitType(int x, int y, int unitType, int arg3);

#define NO_TILE_DIST 9999

/* Moves pos to the nearest tile (by steps along rows and columns) where a
 * unit of unitType can be put. FALSE when there is none. */
BOOL Map_FindNearestTileForUnitType(int unitType, TilePoint* pos) {
  int best;
  int targetX;
  int x;
  int targetY;
  int dist;
  int dy;
  int y;

  targetX = pos->x;
  targetY = pos->y;
  best = NO_TILE_DIST;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (Tile_IsFreeForUnitType(x, y, unitType, 0)) {
        dy = y - targetY < 0 ? -(y - targetY) : y - targetY;
        dist = x - targetX;
        if (dist < 0) {
          dist = -dist;
        }
        dist += dy;
        if (best > dist) {
          pos->x = x;
          best = dist;
          pos->y = y;
        }
      }
    }
  }
  return best != NO_TILE_DIST;
}
