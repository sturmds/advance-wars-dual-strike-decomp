#include "types.h"
#include "game/Unk_02080440.h"
#include "game/neighbor_dir.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns the direction in mask whose neighbor of the acting unit's
 * destination is tile (tileX, tileY), or -1. */
int NeighborDir_FindAtTile(int tileX, int tileY, int mask) {
  int direction;

  for (direction = 0; direction < 4; direction++) {
    if ((mask & data_02166378[direction].mask) != 0 &&
        tileX == STATE->actingUnitDestX + data_02166378[direction].dx &&
        tileY == STATE->actingUnitDestY + data_02166378[direction].dy) {
      return direction;
    }
  }
  return -1;
}
