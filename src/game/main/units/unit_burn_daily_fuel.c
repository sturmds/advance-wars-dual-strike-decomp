#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

BOOL Unit_UsesDailyFuel(MapUnit* unit);
int FuelConsumption(int player, int unitType, int hidden);

/* Start of day: takes the unit's daily fuel, set by the terrain it stands on
 * (8 for a hidden Stealth, 5 for a dived Sub), less the owner's CO fuel
 * saving. Returns TRUE when a plane, copter or ship is left with no fuel. */
BOOL Unit_BurnDailyFuel(MapUnit* unit) {
  int fuel = Ov000_gUnitStats[unit->type].dailyFuel[MAP_TILE_TERRAIN(
      MAP_TILES->tiles[unit->x + MAP_TILES->rowStart[unit->y]])];
  int owner = MAP_UNIT_OWNER(MAP_STATE, unit);

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
