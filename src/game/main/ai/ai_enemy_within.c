#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/player.h"
#include "game/ai/ai_best_damage.h"
#include "game/ai/ai_enemy_within.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* Player slot that owns a unit slot of MapTiles.unitAt (see MAP_UNIT_OWNER). */
#define SLOT_OWNER(slot) (((slot) >> 6) + 1)

BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);

/* TRUE when a unit of another team stands within radius tiles of (x, y),
 * counted as |dx| + |dy|, seen or not (compare Ai_HasVisibleEnemyWithin). */
BOOL Ai_HasEnemyWithin(s16 x, s16 y, int radius) {
  int maxX;
  int minY;
  int tileY;
  int maxY;
  int tileX;
  int minX;
  int dist;
  int slot;

  minX = x - radius;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - radius;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + radius < maxX) {
    maxX = x + radius;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
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
      slot = MAP_TILES->unitAt[MAP_TILE_INDEX(tileX, tileY)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
          MAP_STATE->players[SLOT_OWNER(slot)].team) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* TRUE when a unit of another team stands within radius tiles of (x, y),
 * counted as |dx| + |dy|, and the current player can see it: its tile
 * passes Player_CanSeeUnitAt and Unit_IsVisibleToCurrentPlayer accepts it. */
BOOL Ai_HasVisibleEnemyWithin(s16 x, s16 y, int radius) {
  int maxX;
  int minY;
  int tileY;
  int maxY;
  int tileX;
  int minX;
  int dist;
  int slot;

  minX = x - radius;
  if (minX < 0) {
    minX = 0;
  }
  minY = y - radius;
  if (minY < 0) {
    minY = 0;
  }
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + radius < maxX) {
    maxX = x + radius;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
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
      slot = MAP_TILES->unitAt[MAP_TILE_INDEX(tileX, tileY)];
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[SLOT_OWNER(slot)].team) {
        continue;
      }
      if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, tileX, tileY)) {
        continue;
      }
      if (Unit_IsVisibleToCurrentPlayer(
              MAP_TILES->unitAt[MAP_TILE_INDEX(tileX, tileY)])) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* TRUE when a base whose owner bits equal MAP_STATE->unk_04 (likely the
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
  maxX = MAP_STATE->unk_1c4->mapWidth - 1;
  if (x + radius < maxX) {
    maxX = x + radius;
  }
  maxY = MAP_STATE->unk_1c4->mapHeight - 1;
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
      if (MAP_TILE_TERRAIN(MAP_TILES->tiles[MAP_TILE_INDEX(tileX, tileY)]) !=
          TERRAIN_BASE) {
        continue;
      }
      if (MAP_STATE->unk_04 ==
          (MAP_TILES->tiles[MAP_TILE_INDEX(tileX, tileY)] & 0xe0)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
