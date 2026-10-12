#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* CoPowerBlock.affectsUnit filter: every unit except Oozium. */
BOOL UnitHighlightAllButOozium(MapUnit* unit) {
  return Ov000_gUnitStats[unit->type].unitClass != UNIT_CLASS_ALL;
}
