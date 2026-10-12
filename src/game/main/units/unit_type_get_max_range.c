#include "types.h"
#include "game/unit_stats.h"

/* Base maximum firing range of a unit type, before CO and weather changes. */
int UnitType_GetMaxRange(int unitType) {
  return Ov000_gUnitStats[unitType].maxRange;
}
