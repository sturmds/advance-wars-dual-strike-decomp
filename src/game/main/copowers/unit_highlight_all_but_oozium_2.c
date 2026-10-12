#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* CoPowerBlock.affectsUnit filter: every unit except Oozium. Same code as
 * UnitHighlightAllButOozium; the CO table uses both. */
BOOL UnitHighlightAllButOozium2(MapUnit* unit) {
  return Ov000_gUnitStats[unit->type].unitClass != UNIT_CLASS_ALL;
}
