#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* UnitStats.ammo and UnitStats.fuel of record 0; the original code indexes
 * these two addresses by type * sizeof(UnitStats). */

#define TYPE_AMMO(type) Ov000_gUnitStats[type].ammo
#define TYPE_FUEL(type) Ov000_gUnitStats[type].fuel

/* Mode 1 of MapUnit.serviceWanted (the unit wants supplies): drops the mode
 * once the unit has ammo, or never needed any, and its fuel is over half
 * the tank. */
void AiUnit_ClearSupplyWanted(MapUnit* unit) {
  if (TYPE_AMMO(unit->type) != 0 && unit->ammo == 0) {
    return;
  }
  if ((TYPE_FUEL(unit->type) >> 1) < unit->fuel) {
    unit->serviceWanted = 0;
  }
}
