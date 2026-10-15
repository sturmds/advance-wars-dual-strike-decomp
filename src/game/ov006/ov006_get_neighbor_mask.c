#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: nonzero when the tile at x, y counts as a neighbour */

/* Bit mask of the four neighbours of tile x, y that Ov006_IsLandNeighbor
 * accepts: 8 above, 4 left, 2 right, 1 below. Tiles off the map count as 0. */
int Ov006_GetNeighborMask(int x, int y) {
  int mask = 0;

  if (y > 0) {
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 3;
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 2;
  }
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 1;
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1) {
    mask |= Ov006_IsLandNeighbor(x, y + 1);
  }
  return mask;
}
