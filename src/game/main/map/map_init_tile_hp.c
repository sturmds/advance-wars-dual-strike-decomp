#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* hp a pipe seam starts with */
#define PIPE_SEAM_HP 99

/* Sets the hp of every map tile: 99 on an unowned pipe seam, 0 elsewhere.
 * The declaration order of x and y is needed to match. */
void Map_InitTileHp(void) {
  int x;
  int y;
  int tile;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      tile = x + STATE->mapTiles->rowStart[y];
      if (STATE->mapTiles->tiles[tile] == TERRAIN_PIPE_SEAM) {
        STATE->tileHp[tile] = PIPE_SEAM_HP;
      } else {
        STATE->tileHp[tile] = 0;
      }
    }
  }
}
