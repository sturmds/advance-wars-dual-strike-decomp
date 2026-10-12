#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* Per unit type: nonzero for types that can be supplied. */
extern u8 data_ov000_022f5cac[];

BOOL Tile_HasOwnUnitOfListedType(s16 x, s16 y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the unit can be supplied and one of the four tiles next to it
 * holds a unit of the current player whose type supplies others (the list
 * func_020df2f8 reads; a guess). The start-of-turn resupply uses it. */
BOOL Unit_HasAdjacentSupplier(MapUnit* unit) {
  if (data_ov000_022f5cac[unit->type] == 0) {
    return FALSE;
  }
  if (unit->x != 0 && Tile_HasOwnUnitOfListedType(unit->x - 1, unit->y)) {
    return TRUE;
  }
  if (unit->y != 0 && Tile_HasOwnUnitOfListedType(unit->x, unit->y - 1)) {
    return TRUE;
  }
  if (unit->x < STATE->unk_1c4->mapWidth - 1 &&
      Tile_HasOwnUnitOfListedType(unit->x + 1, unit->y)) {
    return TRUE;
  }
  if (unit->y < STATE->unk_1c4->mapHeight - 1 &&
      Tile_HasOwnUnitOfListedType(unit->x, unit->y + 1)) {
    return TRUE;
  }
  return FALSE;
}
