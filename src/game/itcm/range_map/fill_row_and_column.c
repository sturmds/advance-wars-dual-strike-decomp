#include "types.h"
#include "game/Unk_02080440.h"

#define TILE(x, y) (MAP_STATE->rangeMap[y][x])

/* Writes value into every tile of the row and the column through (x, y),
 * except (x, y) itself. MapObject_MarkFireRange calls it for
 * MAP_OBJECT_LASER. */
void RangeMap_FillRowAndColumn(int x, int y, s8 value) {
  int i;

  for (i = 0; i < MAP_STATE->unk_1c4->mapWidth; i++) {
    if (i != x) {
      TILE(i, y) = value;
    }
  }
  for (i = 0; i < MAP_STATE->unk_1c4->mapHeight; i++) {
    if (i != y) {
      TILE(x, i) = value;
    }
  }
}
