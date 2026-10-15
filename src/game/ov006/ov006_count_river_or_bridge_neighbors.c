#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

#define TERRAIN_AT(x, y) \
  (MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])

int Ov006_IsSpecialCell(
    int x, int y); /* tile x, y holds cell code 0xfc, 0xfd, 0x11c or 0x11d */

/* Counts the four neighbours of tile x, y (above, below, left, right) that
 * are river or bridge, leaving out the special cells of Ov006_IsSpecialCell. */
int Ov006_CountRiverOrBridgeNeighbors(int x, int y) {
  int count = 0;
  u8 terrain;

  if (y > 0) {
    terrain = TERRAIN_AT(x, y - 1);
    if (Ov006_IsSpecialCell(x, y - 1) == 0 &&
        (terrain == TERRAIN_RIVER || terrain == TERRAIN_BRIDGE)) {
      count++;
    }
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    terrain = TERRAIN_AT(x, y + 1);
    if (Ov006_IsSpecialCell(x, y + 1) == 0 &&
        (terrain == TERRAIN_RIVER || terrain == TERRAIN_BRIDGE)) {
      count++;
    }
  }
  if (x > 0) {
    terrain = TERRAIN_AT(x - 1, y);
    if (Ov006_IsSpecialCell(x - 1, y) == 0 &&
        (terrain == TERRAIN_RIVER || terrain == TERRAIN_BRIDGE)) {
      count++;
    }
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    terrain = TERRAIN_AT(x + 1, y);
    if (Ov006_IsSpecialCell(x + 1, y) == 0 &&
        (terrain == TERRAIN_RIVER || terrain == TERRAIN_BRIDGE)) {
      count++;
    }
  }
  return count;
}
