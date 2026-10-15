#include "types.h"
#include "game/Unk_02080440.h"

BOOL DeployCity(u8 tile);

BOOL Map_IsFreeDeployCity(int x, int y) {
  MapTiles* mapTiles = data_027c027c->unk_48[data_027c0284]->mapTiles;
  int index = x + mapTiles->rowStart[y];
  u8 unit = mapTiles->unitAt[index];
  u8 tile = mapTiles->tiles[index];

  if (unit != 0) {
    return FALSE;
  }
  return DeployCity(tile) != 0;
}
