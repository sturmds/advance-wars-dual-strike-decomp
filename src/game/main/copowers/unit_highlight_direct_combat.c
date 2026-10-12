#include "types.h"
#include "game/co_data.h"
#include "game/map_unit.h"

/* Returns UNIT_CLASS_DIRECT_COMBAT, UNIT_CLASS_INDIRECT_COMBAT,
 * UNIT_CLASS_TRANSPORT or UNIT_CLASS_EXPLODES. */
int GetUnitCombatClass(int unitType);

/* CoPowerBlock.affectsUnit filter: direct-attack units that are not inside
 * a transport. */
BOOL UnitHighlightDirectCombat(MapUnit* unit) {
  if (unit->flags & MAP_UNIT_CARRIED) {
    return FALSE;
  }
  return GetUnitCombatClass(unit->type) == UNIT_CLASS_DIRECT_COMBAT;
}
