#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 if tile x, y is on the map and its tile byte equals tile, else 0. */
int Ov006_TileMatches(int x, int y, int tile) {
  if (x < 0 || y < 0 || y > STATE->unk_1c4->mapHeight - 1 ||
      x > STATE->unk_1c4->mapWidth - 1) {
    return 0;
  }

  return tile == STATE->mapTiles->tiles[STATE->mapTiles->rowStart[y] + x];
}
