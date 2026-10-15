#include "types.h"
#include "game/Unk_02080440.h"

/* Clears the range map tiles next to (x, y) in the directions set in
 * dirs: 1 up, 2 down, 4 left, 8 right. */
void RangeMap_ClearAdjacent(int x, int y, int dirs) {
  if (dirs & 4) {
    MAP_STATE->rangeMap[y][x - 1] = 0;
  }
  if (dirs & 8) {
    MAP_STATE->rangeMap[y][x + 1] = 0;
  }
  if (dirs & 1) {
    MAP_STATE->rangeMap[y - 1][x] = 0;
  }
  if (dirs & 2) {
    MAP_STATE->rangeMap[y + 1][x] = 0;
  }
}
