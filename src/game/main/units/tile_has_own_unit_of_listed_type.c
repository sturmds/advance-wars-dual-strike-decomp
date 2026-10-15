#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

int func_020df2f8(
    MapUnit*
        unit); /* nonzero by unit type, from the table data_ov000_022f5ce4 */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit on tile (x, y) belongs to the current player and
 * func_020df2f8 accepts its type. A tile without a unit gives slot 0, which
 * is not in the current player's block of slots. */
BOOL Tile_HasOwnUnitOfListedType(int x, int y) {
  MapUnit* unit =
      &MAP_STATE->units[MAP_STATE->mapTiles
                            ->unitAt[x + MAP_STATE->mapTiles->rowStart[y]]];

  if (MAP_STATE->currentUnitBase == ((unit - MAP_STATE->units) & 0xc0)) {
    if (func_020df2f8(unit)) {
      return TRUE;
    }
  }
  return FALSE;
}
