#include "types.h"
#include "game/Unk_02080440.h"

BOOL Map_IsDeployTile(u8 tile);

/* TRUE when no unit stands on tile (x, y) and Map_IsDeployTile accepts its terrain
 * and owner. The tile byte is read before the unit test to match. */
BOOL Map_IsVacantDeployTile(int x, int y) {
  MapTiles* mapTiles = data_027c027c->unk_48[data_027c0284]->mapTiles;
  int index = x + mapTiles->rowStart[y];
  u8 unit = mapTiles->unitAt[index];
  u8 tile = mapTiles->tiles[index];

  if (unit != 0) {
    return FALSE;
  }
  return Map_IsDeployTile(tile) != 0;
}
