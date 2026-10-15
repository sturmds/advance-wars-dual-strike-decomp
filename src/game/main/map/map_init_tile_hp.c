#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* hp a pipe seam starts with */
#define PIPE_SEAM_HP 99

/* Sets the hp of every map tile: 99 on an unowned pipe seam, 0 elsewhere.
 * The declaration order of x and y is needed to match. */
void Map_InitTileHp(void) {
  int x;
  int y;
  int tile;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      tile = x + MAP_STATE->mapTiles->rowStart[y];
      if (MAP_STATE->mapTiles->tiles[tile] == TERRAIN_PIPE_SEAM) {
        MAP_STATE->tileHp[tile] = PIPE_SEAM_HP;
      } else {
        MAP_STATE->tileHp[tile] = 0;
      }
    }
  }
}
