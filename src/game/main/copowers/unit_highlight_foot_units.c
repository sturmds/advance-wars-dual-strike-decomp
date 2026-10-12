#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* CoPowerBlock.affectsUnit filter: foot units that are not inside a transport. */
BOOL UnitHighlightFootUnits(MapUnit* unit) {
  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  return Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_FOOT;
}
