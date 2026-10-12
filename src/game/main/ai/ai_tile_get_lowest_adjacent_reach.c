#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define NOT_REACHABLE 111

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
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + 1 < maxX) {
    maxX = x + 1;
  }
  best = NOT_REACHABLE;
  maxY = STATE->unk_1c4->mapHeight - 1;
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
      reach = STATE->rangeMap[cy][cx];
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
