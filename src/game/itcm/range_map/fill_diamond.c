#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int abs(int n);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Writes value into every range map tile within radius steps of (x, y),
 * skipping tiles off the map. value is an int narrowed at the store:
 * needed to match (callers declare it s8). */
void RangeMap_FillDiamond(int x, int y, s16 radius, int value) {
  s16 tx;
  s16 ty;
  s16 dy;

  for (dy = -radius, ty = y - radius; ty <= y + radius; ty++, dy++) {
    if (ty >= 0 && ty < STATE->unk_1c4->mapHeight) {
      for (tx = x - radius + abs(dy); tx <= x + radius - abs(dy); tx++) {
        if (tx >= 0 && tx < STATE->unk_1c4->mapWidth) {
          STATE->rangeMap[ty][tx] = (s8)value;
        }
      }
    }
  }
}
