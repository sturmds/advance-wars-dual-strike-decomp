#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s16 MapCell_SetOwner(int x, int y, int tile);
void Map_CountPlayerProperties(void);
void func_020c4018(void);
void func_020c4448(void);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (GAME->mapTiles->tiles[(x) + GAME->mapTiles->rowStart[y]])

/* Gives the tile at (x, y) to the current player (the owner bits are stored
 * unshifted in unk_04), updates its map cell, and refreshes the property
 * counts. */
void Tile_SetOwner(int x, int y) {
  s8 tile = (TILE(x, y) & 0x1f) + GAME->unk_04;

  TILE(x, y) = tile;
  MapCell_SetOwner(x, y, (u8)(tile & 0xe0));
  func_020c4018();
  func_020c4448();
  Map_CountPlayerProperties();
}
