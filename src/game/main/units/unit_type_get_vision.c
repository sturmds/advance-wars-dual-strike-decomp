#include "types.h"
#include "game/unit_stats.h"

/* Base vision of a unit type, before CO, weather and terrain changes. */
int UnitType_GetVision(int unitType) {
  return Ov000_gUnitStats[unitType].vision;
}
