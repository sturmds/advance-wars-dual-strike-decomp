#include "types.h"
#include "game/map_unit.h"

int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);

/* Refills ammo and fuel without charging anyone. Returns the two counts
 * added up (the shown-HP points Unit_Rearm and Unit_Refuel report). */
int Unit_ResupplyUnpaid(MapUnit* unit) {
  int total = 0;

  total += Unit_Rearm(unit, FALSE);
  total += Unit_Refuel(unit, FALSE);
  return total;
}
