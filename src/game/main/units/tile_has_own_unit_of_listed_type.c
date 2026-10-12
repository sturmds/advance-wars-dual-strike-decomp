#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int func_020df2f8(
    MapUnit*
        unit); /* nonzero by unit type, from the table data_ov000_022f5ce4 */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit on tile (x, y) belongs to the current player and
 * func_020df2f8 accepts its type. A tile without a unit gives slot 0, which
 * is not in the current player's block of slots. */
BOOL Tile_HasOwnUnitOfListedType(int x, int y) {
  MapUnit* unit =
      &STATE->units[STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]]];

  if (STATE->currentUnitBase == ((unit - STATE->units) & 0xc0)) {
    if (func_020df2f8(unit)) {
      return TRUE;
    }
  }
  return FALSE;
}
