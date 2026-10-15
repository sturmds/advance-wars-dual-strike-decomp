#include "types.h"
#include "game/Unk_02080440.h"

int abs(int n);

/* Writes value into every range map tile within radius steps of (x, y),
 * skipping tiles off the map. value is an int narrowed at the store:
 * needed to match (callers declare it s8). */
void RangeMap_FillDiamond(int x, int y, s16 radius, int value) {
  s16 tx;
  s16 ty;
  s16 dy;

  for (dy = -radius, ty = y - radius; ty <= y + radius; ty++, dy++) {
    if (ty >= 0 && ty < MAP_STATE->unk_1c4->mapHeight) {
      for (tx = x - radius + abs(dy); tx <= x + radius - abs(dy); tx++) {
        if (tx >= 0 && tx < MAP_STATE->unk_1c4->mapWidth) {
          MAP_STATE->rangeMap[ty][tx] = (s8)value;
        }
      }
    }
  }
}
