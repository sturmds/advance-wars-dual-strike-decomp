#include "types.h"
#include "game/Unk_02080440.h"

BOOL Tile_IsFreeForUnitType(s16 x, s16 y, int unitType, BOOL checkUnitAt);

/* Direction bits of the result */
#define DROP_UP 0x01
#define DROP_DOWN 0x02
#define DROP_LEFT 0x04
#define DROP_RIGHT 0x08

/* Bits for the tiles next to (x, y) where the unit in slot could be put
 * down: on the map, no unit there, and terrain its move type can enter.
 * Slot 0: none. */

s8 Unit_GetDropDirections(int x, int y, int slot) {
  s8 dirs;
  MapUnit* unit;

  dirs = 0;
  if (slot == 0) {
    return dirs;
  }
  unit = &MAP_STATE->units[slot];
  if (Tile_IsFreeForUnitType(x - 1, y, unit->type, TRUE) == TRUE) {
    dirs |= DROP_LEFT;
  }
  if (Tile_IsFreeForUnitType(x + 1, y, unit->type, TRUE) == TRUE) {
    dirs |= DROP_RIGHT;
  }
  if (Tile_IsFreeForUnitType(x, y - 1, unit->type, TRUE) == TRUE) {
    dirs |= DROP_UP;
  }
  if (Tile_IsFreeForUnitType(x, y + 1, unit->type, TRUE) == TRUE) {
    dirs |= DROP_DOWN;
  }
  return dirs;
}
