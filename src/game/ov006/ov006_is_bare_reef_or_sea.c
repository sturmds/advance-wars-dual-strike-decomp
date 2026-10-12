#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 if the tile at (x, y) is unowned reef or sea, else 0. */
int Ov006_IsBareReefOrSea(int x, int y) {
  u8 tile = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];

  if (tile == TERRAIN_REEF || tile == TERRAIN_SEA) {
    return 1;
  }
  return 0;
}
