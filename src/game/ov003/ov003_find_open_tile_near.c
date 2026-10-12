#include "types.h"
#include "game/ov003_unit.h"

int Ov003_CanPlaceUnit(int tile, int x, int y);

/* Looks for a tile that Ov003_CanPlaceUnit accepts for a unit of the given
 * tile code, starting at (*x, *y) and going outwards ring by ring (ring 1 is
 * the four neighbours). Each ring is walked in four runs. On success it moves
 * *x and *y to the tile found and returns 1. The x offset of a run is the
 * variable it counts with; the other one is the y offset, ring minus it or
 * plus it. Needed to match: the first run counts with j, the others with i. */
int Ov003_FindOpenTileNear(int tile, int* x, int* y) {
  int ring;
  int j;
  int i;

  if (Ov003_CanPlaceUnit(tile, *x, *y) == 1) {
    return 1;
  }
  ring = 1;
  do {
    for (j = -ring; j < 0; j++) {
      i = ring + j;
      if (Ov003_CanPlaceUnit(tile, *x + j, *y - i) == 1) {
        *x += j;
        *y -= i;
        return 1;
      }
    }
    for (i = 0; i < ring; i++) {
      j = ring - i;
      if (Ov003_CanPlaceUnit(tile, *x + i, *y - j) == 1) {
        *x += i;
        *y -= j;
        return 1;
      }
    }
    for (i = ring; i > 0; i--) {
      j = ring - i;
      if (Ov003_CanPlaceUnit(tile, *x + i, *y + j) == 1) {
        *x += i;
        *y += j;
        return 1;
      }
    }
    for (i = 0; i > -ring; i--) {
      j = ring + i;
      if (Ov003_CanPlaceUnit(tile, *x + i, *y + j) == 1) {
        *x += i;
        *y += j;
        return 1;
      }
    }
    ring++;
  } while (ring < data_ov003_02364a88->width + data_ov003_02364a88->height);
  return 0;
}
