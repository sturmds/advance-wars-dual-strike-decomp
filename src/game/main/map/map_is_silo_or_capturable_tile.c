#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Ov000_gUrbanTerrain of the tile's terrain, or 0 when the tile belongs to
 * the acting unit's team. */
int Map_IsCapturableByActingUnit(s16 x, s16 y);

/* Nonzero when the acting unit has something to do on tile (x, y): a missile
 * silo that has not been fired, or a property its team does not own. */
BOOL Map_IsSiloOrCapturableTile(int x, int y) {
  if (MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]]) ==
      TERRAIN_SILO) {
    return TRUE;
  }
  return Map_IsCapturableByActingUnit(x, y);
}
