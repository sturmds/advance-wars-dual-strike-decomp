#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* CoPowerBlock.affectsUnit filter: planes and copters that are not inside
 * a transport. Eagle's day-to-day block uses it. */
BOOL UnitHighlightAirUnits(MapUnit* unit) {
  u8 unitClass;

  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  unitClass = Ov000_gUnitStats[unit->type].unitClass;
  /* Range check for PLANE..COPTER. Needed to match: adding (u8)-3 gives
     * the original `add #0xfd`; subtracting 3 gives `sub`. */
  unitClass += (u8)-UNIT_CLASS_PLANE;
  return unitClass <= UNIT_CLASS_COPTER - UNIT_CLASS_PLANE;
}
