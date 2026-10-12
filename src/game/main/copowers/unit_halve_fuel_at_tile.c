#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Halves the fuel of the unit on tile (x, y), rounding down. Same filters as
 * Unit_DamageAtTile: nothing off the map, on an empty tile, for a carried
 * unit or for Oozium. PowerDamage_HitNextEnemy calls it for Drake's powers. */
void Unit_HalveFuelAtTile(int x, int y) {
  MapUnit* unit;
  int slot;
  int tile;

  if (x < 0 || x >= STATE->unk_1c4->mapWidth) {
    return;
  }
  if (y < 0 || y >= STATE->unk_1c4->mapHeight) {
    return;
  }
  tile = x + STATE->mapTiles->rowStart[y];
  slot = STATE->mapTiles->unk_508[tile];
  unit = &STATE->units[slot];
  if (unit->type == UNIT_NONE) {
    return;
  }
  if (unit->flags & MAP_UNIT_CARRIED) {
    return;
  }
  if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL) {
    return;
  }
  unit->fuel /= 2;
}
