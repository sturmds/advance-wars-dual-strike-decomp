#include "types.h"
#include "game/Unk_02080440.h"

/* 1 if tile x, y is on the map and its tile byte equals tile, else 0. */
int Ov006_TileMatches(int x, int y, int tile) {
  if (x < 0 || y < 0 || y > MAP_STATE->unk_1c4->mapHeight - 1 ||
      x > MAP_STATE->unk_1c4->mapWidth - 1) {
    return 0;
  }

  return tile ==
         MAP_STATE->mapTiles->tiles[MAP_STATE->mapTiles->rowStart[y] + x];
}
