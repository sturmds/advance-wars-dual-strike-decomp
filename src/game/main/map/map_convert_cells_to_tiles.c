#include "types.h"
#include "game/Unk_02080440.h"

void func_020c4448(void);
void func_020c4018(void);

/* Sets every MapTiles.tiles byte from the cell code of the tile (through
 * tileCodeTerrain), then rebuilds the property lists (func_020c4448) and
 * runs func_020c4018. */
void Map_ConvertCellsToTiles(void) {
  int x;
  int y;
  int tile;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      tile = x + MAP_STATE->mapTiles->rowStart[y];
      MAP_STATE->mapTiles->tiles[tile] =
          MAP_STATE->tileCodeTerrain[MAP_STATE->mapTiles->mapCells[tile]];
    }
  }
  func_020c4448();
  func_020c4018();
}
