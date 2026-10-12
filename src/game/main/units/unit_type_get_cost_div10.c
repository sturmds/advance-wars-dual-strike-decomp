#include "types.h"
#include "game/unit_stats.h"

/* Cost of a unit type in funds / 10, before any CO cost change. */
u32 UnitType_GetCostDiv10(int unitType) {
  return Ov000_gUnitStats[unitType].costDiv10;
}
