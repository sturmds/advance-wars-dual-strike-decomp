#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_IsSpecialCell(int x, int y);
int Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: nonzero when the tile at x, y counts as a neighbour */

/* Counts the four neighbours of tile x, y that Ov006_IsLandNeighbor accepts.
 * Returns -1 for a reef tile, for one of the special cells of
 * Ov006_IsSpecialCell, and when all four neighbours count. */
int Ov006_CountMatchingNeighbors(int x, int y) {
  int count;

  if (MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] ==
          TERRAIN_REEF ||
      Ov006_IsSpecialCell(x, y) != 0) {
    return -1;
  }
  count = 0;
  /* y > 1, not y > 0: the tile above is skipped on row 1 too */
  if (y > 1) {
    count += Ov006_IsLandNeighbor(x, y - 1);
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    count += Ov006_IsLandNeighbor(x, y + 1);
  }
  if (x > 0) {
    count += Ov006_IsLandNeighbor(x - 1, y);
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    count += Ov006_IsLandNeighbor(x + 1, y);
  }
  if (count == 4) {
    count = -1;
  }
  return count;
}
