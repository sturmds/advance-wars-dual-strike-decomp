#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

/* Sets MAP_UNIT_PARALYSED on the unit on tile (x, y), and also
 * MAP_UNIT_WAITED when it belongs to player. Same filters as
 * Unit_DamageAtTile: nothing off the map, on an empty tile, for a carried
 * unit or for Oozium. func_020e462c calls both for each tile around a
 * target, after 30 damage (3 hp). */
void Unit_ParalyseAtTile(int x, int y, int player) {
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
  unit->flags |= MAP_UNIT_PARALYSED;
  if (MAP_UNIT_OWNER(MAP_STATE, unit) == player) {
    unit->flags |= MAP_UNIT_WAITED;
  }
}
