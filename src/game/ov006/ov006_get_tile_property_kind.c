#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_GetPropertyKind(int terrain);

/* Classifies the terrain of the tile at (x, y), see Ov006_GetPropertyKind. */
int Ov006_GetTilePropertyKind(int x, int y) {
  return Ov006_GetPropertyKind(MAP_TILE_TERRAIN(
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]]));
}
