#include "types.h"
#include "game/Unk_02080440.h"

/* Fills MapTiles.rowStart: the index in the tile arrays of each row's first
 * tile, row * map width. */
void Map_BuildRowStarts(void) {
  u8 row;

  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    MAP_STATE->mapTiles->rowStart[row] = row * MAP_STATE->unk_1c4->mapWidth;
  }
}
