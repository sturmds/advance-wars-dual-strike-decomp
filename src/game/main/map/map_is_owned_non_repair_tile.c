#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Per terrain id: which units the tile repairs. 1 ground (city, HQ, base),
 * 2 air (airport), 3 sea (port), 0 none. */
extern u8 gTerrainSupplyKind[];

/* TRUE when tile (x, y) has the owner bits in Unk_02080440_48.unk_04 and its
 * terrain repairs no units. */
BOOL Map_IsOwnedNonRepairTile(int x, int y) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  u8 tile = state->mapTiles->tiles[x + state->mapTiles->rowStart[y]];

  if (state->unk_04 != (tile & 0xe0)) {
    return FALSE;
  }
  return gTerrainSupplyKind[MAP_TILE_TERRAIN(tile)] == 0;
}
