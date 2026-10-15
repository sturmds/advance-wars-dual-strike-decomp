#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

BOOL Tile_HasTransportToBoard(int x, int y);
BOOL Unit_CanJoin(MapUnit* unit, MapUnit* target);

/* Whether the acting unit, if it belongs to the current player, can end its
 * move on (x, y): the tile must be in its move range (range map 0 to 110)
 * and be empty, hold a transport it can board or a unit it can join, or,
 * for Oozium, hold a unit of another team. */
BOOL ActingUnit_CanMoveTo(int x, int y) {
  Unk_02080440_48* state = MAP_STATE;
  u8 slot = state->mapTiles->unitAt[state->mapTiles->rowStart[y] + x];

  if (state->currentUnitBase != (state->actingUnitIndex & 0xc0)) {
    return FALSE;
  }
  if (state->rangeMap[y][x] < 0 || state->rangeMap[y][x] > 110) {
    return FALSE;
  }
  if (slot == 0) {
    return TRUE;
  }
  if (Tile_HasTransportToBoard(x, y) == TRUE) {
    return TRUE;
  }
  if (Unit_CanJoin(MAP_STATE->actingUnit, &MAP_STATE->units[slot]) == TRUE) {
    return TRUE;
  }
  if (MAP_STATE->actingUnit->type == UNIT_OOZIUM &&
      MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, MAP_STATE->actingUnit)]
              .team !=
          MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, &MAP_STATE->units[slot])]
              .team) {
    return TRUE;
  }
  return FALSE;
}
