#include "types.h"
#include "game/map_object.h"

/* Pixel offset of the muzzle from the cannon's tile, per cannon kind.
 * Other kinds leave x and y alone. */
void CannonShot_GetMuzzleOffset(int* x, int* y, int kind) {
  if (kind == MAP_OBJECT_BLACK_CANNON) {
    *x = 24;
    *y = 48;
  }
  if (kind == MAP_OBJECT_MINICANNON) {
    *x = 8;
    *y = 16;
  }
}
