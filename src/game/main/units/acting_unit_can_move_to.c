#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Tile_HasTransportToBoard(int x, int y);
BOOL Unit_CanJoin(MapUnit* unit, MapUnit* target);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Whether the acting unit, if it belongs to the current player, can end its
 * move on (x, y): the tile must be in its move range (range map 0 to 110)
 * and be empty, hold a transport it can board or a unit it can join, or,
 * for Oozium, hold a unit of another team. */
BOOL ActingUnit_CanMoveTo(int x, int y) {
  Unk_02080440_48* state = GAME;
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
  if (Unit_CanJoin(GAME->actingUnit, &GAME->units[slot]) == TRUE) {
    return TRUE;
  }
  if (GAME->actingUnit->type == UNIT_OOZIUM &&
      GAME->players[MAP_UNIT_OWNER(GAME, GAME->actingUnit)].team !=
          GAME->players[MAP_UNIT_OWNER(GAME, &GAME->units[slot])].team) {
    return TRUE;
  }
  return FALSE;
}
