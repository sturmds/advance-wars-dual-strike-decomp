#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/tile_point.h"

BOOL Player_HasHq(int player);
BOOL Map_FindNearestTileForUnitType(int unitType, TilePoint* pos);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Looks for a tile on the other map of a dual-front battle where the unit
 * could go: the free tile nearest to its owner's HQ there, or, when the
 * state's unk_86 is set, nearest to the tile already in pos. Writes the
 * tile to pos. FALSE when the owner has no HQ on the other map (unk_86
 * clear) or no tile fits. */
BOOL Unit_FindOtherMapTile(MapUnit* unit, TilePoint* pos) {
  int owner = MAP_UNIT_OWNER(STATE, unit);
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  BOOL found;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  if (STATE->unk_86 == 0) {
    if (!Player_HasHq(owner)) {
      found = FALSE;
    } else {
      found = TRUE;
    }
    if (!found) {
      goto restore;
    }
    pos->x = STATE->players[owner].hqX & 0x7f;
    pos->y = STATE->players[owner].hqY & 0x7f;
  }
  found = Map_FindNearestTileForUnitType(unit->type, pos) != 0;
restore:
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return found;
}
