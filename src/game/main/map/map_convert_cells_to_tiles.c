#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_020c4448(void);
void func_020c4018(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets every MapTiles.tiles byte from the cell code of the tile (through
 * tileCodeTerrain), then rebuilds the property lists (func_020c4448) and
 * runs func_020c4018. */
void Map_ConvertCellsToTiles(void) {
  int x;
  int y;
  int tile;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      tile = x + STATE->mapTiles->rowStart[y];
      STATE->mapTiles->tiles[tile] =
          STATE->tileCodeTerrain[STATE->mapTiles->mapCells[tile]];
    }
  }
  func_020c4448();
  func_020c4018();
}
