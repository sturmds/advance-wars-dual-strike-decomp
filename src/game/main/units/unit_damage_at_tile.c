#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

/* Takes damage hp off the unit on tile (x, y), leaving it at 1 hp at least.
 * Does nothing off the map, on an empty tile, for a carried unit or for
 * Oozium (the unit class that no CO row covers). Silo hits call it. */
void Unit_DamageAtTile(int x, int y, int damage) {
  MapUnit* unit;
  int slot;
  int tile;

  if (x < 0 || x >= MAP_STATE->unk_1c4->mapWidth) {
    return;
  }
  if (y < 0 || y >= MAP_STATE->unk_1c4->mapHeight) {
    return;
  }
  tile = x + MAP_STATE->mapTiles->rowStart[y];
  slot = MAP_STATE->mapTiles->unk_508[tile];
  unit = &MAP_STATE->units[slot];
  if (unit->type == UNIT_NONE) {
    return;
  }
  if (unit->flags & MAP_UNIT_CARRIED) {
    return;
  }
  if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL) {
    return;
  }
  if (unit->hp < damage + 1) {
    unit->hp = 1;
  } else {
    unit->hp -= damage;
  }
}
