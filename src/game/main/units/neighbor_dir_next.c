#include "types.h"
#include "game/neighbor_dir.h"

/* Steps forward (wrapping 0-3) from direction to the nearest direction whose
 * bit is in mask. Loops forever when mask holds no direction bit. */
int NeighborDir_Next(int direction, int mask) {
  do {
    direction = (direction + 1) & 3;
  } while ((mask & data_02166378[direction].mask) == 0);
  return direction;
}
