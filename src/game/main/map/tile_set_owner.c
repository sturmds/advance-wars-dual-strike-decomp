#include "types.h"
#include "game/Unk_02080440.h"

s16 MapCell_SetOwner(int x, int y, int tile);
void Map_CountPlayerProperties(void);
void func_020c4018(void);
void func_020c4448(void);

/* Gives the tile at (x, y) to the current player (the owner bits are stored
 * unshifted in unk_04), updates its map cell, and refreshes the property
 * counts. */
void Tile_SetOwner(int x, int y) {
  s8 tile = (MAP_TILE_AT(x, y) & 0x1f) + MAP_STATE->unk_04;

  MAP_TILE_AT(x, y) = tile;
  MapCell_SetOwner(x, y, (u8)(tile & 0xe0));
  func_020c4018();
  func_020c4448();
  Map_CountPlayerProperties();
}
