#include "types.h"
#include "game/Unk_02080440.h"

#define TILE(x, y) (MAP_STATE->rangeMap[y][x])

/* Marks the four tiles around (x, y) as reached (0), whatever they held.
 * The Repair and Supply menus call it for the acting unit's destination. */
void RangeMap_SetAdjacentReachable(int x, int y) {
  if (x > 0) {
    TILE(x - 1, y) = 0;
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    TILE(x + 1, y) = 0;
  }
  if (y > 0) {
    TILE(x, y - 1) = 0;
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    TILE(x, y + 1) = 0;
  }
}
