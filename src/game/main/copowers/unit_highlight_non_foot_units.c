#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* CoPowerBlock.affectsUnit filter for Eagle's powers: non-foot units that
 * have already acted this turn and are not inside a transport. Those are
 * the units his power lets move again. */
BOOL UnitHighlightNonFootUnits(MapUnit* unit) {
  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_FOOT) {
    return FALSE;
  }
  return (unit->flags & MAP_UNIT_WAITED) != 0;
}
