#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_UsesDailyFuel(MapUnit* unit);
int FuelConsumption(int player, int unitType, int hidden);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define MAP (GAME->mapTiles)

/* Start of day: takes the unit's daily fuel, set by the terrain it stands on
 * (8 for a hidden Stealth, 5 for a dived Sub), less the owner's CO fuel
 * saving. Returns TRUE when a plane, copter or ship is left with no fuel. */
BOOL Unit_BurnDailyFuel(MapUnit* unit) {
  int fuel = Ov000_gUnitStats[unit->type].dailyFuel[MAP_TILE_TERRAIN(
      MAP->tiles[unit->x + MAP->rowStart[unit->y]])];
  int owner = MAP_UNIT_OWNER(GAME, unit);

  if (!Unit_UsesDailyFuel(unit)) {
    return FALSE;
  }
  if (unit->flags & MAP_UNIT_DIVED) {
    /* needed to match: a switch with the default case first */
    switch (unit->type) {
      case UNIT_SUB:
      default:
        fuel = 5;
        break;
      case UNIT_STEALTH:
        fuel = 8;
        break;
    }
  }
  fuel -= FuelConsumption(owner, unit->type, unit->flags & MAP_UNIT_DIVED);
  if (fuel < 0) {
    fuel = 0;
  }
  if (unit->fuel <= fuel) {
    unit->fuel = 0;
  } else {
    unit->fuel -= fuel;
  }
  if (unit->fuel == 0 && (Ov000_gUnitStats[unit->type].moveGroup &
                          (MOVE_GROUP_AIR | MOVE_GROUP_SEA))) {
    return TRUE;
  }
  return FALSE;
}
