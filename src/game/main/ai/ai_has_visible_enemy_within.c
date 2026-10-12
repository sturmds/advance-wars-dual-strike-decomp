#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_CanSeeUnitAt(u16 player, s16 x,
                         s16 y); /* fog, weather and terrain check of a tile */
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* Player slot that owns a unit slot of MapTiles.unitAt (see MAP_UNIT_OWNER). */
#define SLOT_OWNER(slot) (((slot) >> 6) + 1)

/* TRUE when a unit of another team stands within radius tiles of (x, y),
 * counted as |dx| + |dy|, and the current player can see it: its tile
 * passes Player_CanSeeUnitAt and Unit_IsVisibleToCurrentPlayer accepts it. */
BOOL Ai_HasVisibleEnemyWithin(int x, int y, int radius) {
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
      slot = MAP->unitAt[TILE_INDEX(tileX, tileY)];
      if (slot == 0) {
        continue;
      }
      if (STATE->players[STATE->currentPlayer].team ==
          STATE->players[SLOT_OWNER(slot)].team) {
        continue;
      }
      if (!Player_CanSeeUnitAt(STATE->currentPlayer, tileX, tileY)) {
        continue;
      }
      if (Unit_IsVisibleToCurrentPlayer(
              MAP->unitAt[TILE_INDEX(tileX, tileY)])) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
