#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the terrain at (x, y) is a silo. Needed to match: a one-case
 * switch, not an if, gives the branches the original has. */
BOOL Map_IsSiloTile(int x, int y) {
  switch (MAP_TILE_TERRAIN(
      STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]])) {
    case TERRAIN_SILO:
      return TRUE;
  }
  return FALSE;
}
