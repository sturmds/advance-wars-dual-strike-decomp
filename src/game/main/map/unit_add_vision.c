#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

int Player_GetUnitVision(int playerIndex, int unitType);

/* Adds delta to the vision grid count of each tile the unit sees, in the grids
 * its owner's Player.visionGrids bits pick. Infantry and Mech on a mountain see 3
 * tiles further. A selected unit that is not locked adds nothing. */
void Unit_AddVision(MapUnit* unit, s8 delta) {
  int mountainBonus = 0;
  Unk_02080440_48* state;
  int slotGroup;
  int owner;

  if (unit->type == UNIT_NONE) {
    return;
  }
  if ((unit->flags & MAP_UNIT_SELECTED) && !(unit->flags & MAP_UNIT_LOCKED)) {
    return;
  }
  if (unit->type <= UNIT_MECH &&
      MAP_TILE_TERRAIN(
          MAP_STATE->mapTiles
              ->tiles[unit->x + MAP_STATE->mapTiles->rowStart[unit->y]]) ==
          TERRAIN_MOUNTAIN) {
    mountainBonus = 3;
  }
  state = MAP_STATE;
  /* needed to match: players is indexed from slot 1 by the 0-based group */
  slotGroup = (unit - state->units) >> 6;
  owner = slotGroup + 1;
  Vision_AddArea(unit->x, unit->y,
                 mountainBonus + Player_GetUnitVision(owner, unit->type),
                 (state->players + 1)[slotGroup].visionGrids, delta, owner);
}
