#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when tile (x, y) holds another unit of the current player, other than
 * the acting unit and never Oozium, that is below 10 shown hp or short of
 * ammo or fuel. A tile off the map has none. */
BOOL Tile_HasUnitToRepair(s16 x, s16 y) {
  MapUnit* unit;
  u8 slot;
  u32 shownHp; /* needed to match: unsigned compare */

  if (x < 0 || x >= STATE->unk_1c4->mapWidth || y < 0 ||
      y >= STATE->unk_1c4->mapHeight) {
    return FALSE;
  }
  slot = STATE->mapTiles->unk_508[x + STATE->mapTiles->rowStart[y]];
  if (slot == 0) {
    return FALSE;
  }
  if ((slot & 0xc0) != STATE->currentUnitBase) {
    return FALSE;
  }
  unit = &STATE->units[slot];
  if (unit == STATE->actingUnit) {
    return FALSE;
  }
  if (unit->type == UNIT_OOZIUM) {
    return FALSE;
  }
  if (unit->hp == 0) {
    shownHp = 0;
  } else {
    shownHp = HwDiv32(unit->hp - 1, 10) + 1;
  }
  if (shownHp < 10) {
    return TRUE;
  }
  if (Ov000_gUnitStats[unit->type].ammo > unit->ammo ||
      Ov000_gUnitStats[unit->type].fuel > unit->fuel) {
    return TRUE;
  }
  return FALSE;
}
