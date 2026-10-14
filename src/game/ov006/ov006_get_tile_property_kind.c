#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_GetPropertyKind(int terrain);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Classifies the terrain of the tile at (x, y), see Ov006_GetPropertyKind. */
int Ov006_GetTilePropertyKind(int x, int y) {
  return Ov006_GetPropertyKind(MAP_TILE_TERRAIN(
      STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]]));
}
