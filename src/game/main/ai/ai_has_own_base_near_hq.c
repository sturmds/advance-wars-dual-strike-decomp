#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_HasHq(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define HQ_X (STATE->players[STATE->currentPlayer].hqX & 0x7f)
#define HQ_Y (STATE->players[STATE->currentPlayer].hqY & 0x7f)

#define BASE_NEAR_HQ_RANGE 4

/* TRUE when the current player has an HQ and a base of owner unk_04 lies
 * within BASE_NEAR_HQ_RANGE tiles of it (Manhattan distance). Same body as
 * Ai_HasOwnBaseWithin, with the HQ read again at each use.
 * Needed to match: hqRight and hqBottom are computed before the map width
 * and height loads (hqRight found by tools/permuter.py). */
BOOL Ai_HasOwnBaseNearHq(void) {
  int maxY;
  int maxX;
  int y;
  int minY;
  int minX;
  int x;
  int dist;
  int hqRight;
  int hqY;
  int hqBottom;

  if (!Player_HasHq(STATE->currentPlayer)) {
    return FALSE;
  }
  minX = HQ_X - BASE_NEAR_HQ_RANGE;
  if (minX < 0) {
    minX = 0;
  }
  hqY = HQ_Y;
  minY = hqY - BASE_NEAR_HQ_RANGE;
  if (minY < 0) {
    minY = 0;
  }
  hqRight = HQ_X + BASE_NEAR_HQ_RANGE;
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (hqRight < maxX) {
    maxX = hqRight;
  }
  hqBottom = hqY + BASE_NEAR_HQ_RANGE;
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (hqBottom < maxY) {
    maxY = hqBottom;
  }
  for (y = minY; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      dist = 0;
      if (HQ_X - x > 0) {
        dist += HQ_X - x;
      } else {
        dist += x - HQ_X;
      }
      if (HQ_Y - y > 0) {
        dist += HQ_Y - y;
      } else {
        dist += y - HQ_Y;
      }
      if (dist < 0 || dist > BASE_NEAR_HQ_RANGE) {
        continue;
      }
      if (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) ==
              TERRAIN_BASE &&
          STATE->unk_04 == (STATE->mapTiles->tiles[TILE(x, y)] & 0xe0)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
