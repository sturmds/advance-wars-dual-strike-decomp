#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* TRUE when a base whose owner bits equal STATE->unk_04 (likely the
 * current player) lies within radius tiles of (x, y), counted as
 * |dx| + |dy| with no move costs. */
BOOL Ai_HasOwnBaseWithin(int x, int y, int radius) {
  int maxX;
  int minY;
  int tileY;
  int maxY;
  int tileX;
  int minX;
  int dist;

  minX = x - radius;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - radius;
  if (minY < 0) {
    minY = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + radius < maxX) {
    maxX = x + radius;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + radius < maxY) {
    maxY = y + radius;
  }
  for (tileY = minY; tileY <= maxY; tileY++) {
    for (tileX = minX; tileX <= maxX; tileX++) {
      dist = 0;
      if (x - tileX > 0) {
        dist += x - tileX;
      } else {
        dist += tileX - x;
      }
      if (y - tileY > 0) {
        dist += y - tileY;
      } else {
        dist += tileY - y;
      }
      if (dist < 0 || dist > radius) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(tileX, tileY)]) !=
          TERRAIN_BASE) {
        continue;
      }
      if (STATE->unk_04 == (MAP->tiles[TILE_INDEX(tileX, tileY)] & 0xe0)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
