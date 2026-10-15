#include "types.h"
#include "game/Unk_02080440.h"
#include "game/neighbor_dir.h"

/* Returns the direction in mask whose neighbor of the acting unit's
 * destination is tile (tileX, tileY), or -1. */
int NeighborDir_FindAtTile(int tileX, int tileY, int mask) {
  int direction;

  for (direction = 0; direction < NEIGHBOR_DIR_COUNT; direction++) {
    if ((mask & data_02166378[direction].mask) != 0 &&
        tileX == MAP_STATE->actingUnitDestX + data_02166378[direction].dx &&
        tileY == MAP_STATE->actingUnitDestY + data_02166378[direction].dy) {
      return direction;
    }
  }
  return -1;
}
