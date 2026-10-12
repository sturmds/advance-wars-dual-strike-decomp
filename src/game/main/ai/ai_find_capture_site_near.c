#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL Ai_IsCaptureSiteTaken(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
/* the Unk_02080440_6088 entry of the property on a tile */
#define PROPERTY_AT(x, y) \
  (STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(x, y)]])

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
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + distance < maxX) {
    maxX = x + distance;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
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
      switch (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(cx, cy)])) {
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
