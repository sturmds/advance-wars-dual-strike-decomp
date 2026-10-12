#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TERRAIN_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

int Ov006_IsSpecialCell(
    int x, int y); /* tile x, y holds cell code 0xfc, 0xfd, 0x11c or 0x11d */

/* Counts the four neighbours of tile x, y (above, below, left, right) that
 * are river, leaving out the special cells of Ov006_IsSpecialCell. Unlike
 * Ov006_CountRiverOrBridgeNeighbors, bridges do not count. */
int Ov006_CountRiverNeighbors(int x, int y) {
  int count = 0;

  if (y > 0 && Ov006_IsSpecialCell(x, y - 1) == 0 &&
      TERRAIN_AT(x, y - 1) == TERRAIN_RIVER) {
    count++;
  }
  if (y < STATE->unk_1c4->mapHeight - 1 && Ov006_IsSpecialCell(x, y + 1) == 0 &&
      TERRAIN_AT(x, y + 1) == TERRAIN_RIVER) {
    count++;
  }
  if (x > 0 && Ov006_IsSpecialCell(x - 1, y) == 0 &&
      TERRAIN_AT(x - 1, y) == TERRAIN_RIVER) {
    count++;
  }
  if (x < STATE->unk_1c4->mapWidth - 1 && Ov006_IsSpecialCell(x + 1, y) == 0 &&
      TERRAIN_AT(x + 1, y) == TERRAIN_RIVER) {
    count++;
  }
  return count;
}
