#include "types.h"
#include "game/co_data.h"

/* CoPowerBlock.unitBonus for Eagle's powers: the unit can move again.
 * Nothing here checks the unit type, so the non-infantry choice probably
 * comes from CoPowerBlock.affectsUnit (not checked yet). */
void UnitNonInfantryMove(MapUnit* unit) {
  unit->flags &= ~(MAP_UNIT_WAITED | MAP_UNIT_PARALYSED);
}
