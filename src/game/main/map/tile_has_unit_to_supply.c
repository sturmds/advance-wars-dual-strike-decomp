#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* Per unit type: nonzero for types that can be supplied (guess; read only here and in Unit_HasAdjacentSupplier). */
extern u8 data_ov000_022f5cac[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when tile (x, y) holds another unit of the current player, other than
 * the acting unit, that is short of ammo or fuel. A tile off the map has none. */
BOOL Tile_HasUnitToSupply(s16 x, s16 y) {
  MapUnit* unit;
  u8 slot;

  if (x < 0 || x >= STATE->unk_1c4->mapWidth || y < 0 ||
      y >= STATE->unk_1c4->mapHeight) {
    return FALSE;
  }
  slot = STATE->mapTiles->unk_508[x + STATE->mapTiles->rowStart[y]];
  if (slot == 0) {
    return FALSE;
  }
  /* slot & 0xc0 is the player's block of 64 unit slots */
  if ((slot & 0xc0) != STATE->currentUnitBase) {
    return FALSE;
  }
  unit = &STATE->units[slot];
  if (unit == STATE->actingUnit) {
    return FALSE;
  }
  if (data_ov000_022f5cac[unit->type] == 0) {
    return FALSE;
  }
  if (unit->ammo == Ov000_gUnitStats[unit->type].ammo) {
    if (unit->fuel == Ov000_gUnitStats[unit->type].fuel) {
      return FALSE;
    }
  }
  return TRUE;
}
