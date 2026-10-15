#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* TRUE when the terrain at (x, y) is a silo. Needed to match: a one-case
 * switch, not an if, gives the branches the original has. */
BOOL Map_IsSiloTile(int x, int y) {
  switch (MAP_TILE_TERRAIN(
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]])) {
    case TERRAIN_SILO:
      return TRUE;
  }
  return FALSE;
}
