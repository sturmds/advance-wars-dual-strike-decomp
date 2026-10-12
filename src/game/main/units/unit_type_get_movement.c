#include "types.h"
#include "game/unit_stats.h"

/* Base movement of a unit type, before CO and weather changes. */
int UnitType_GetMovement(int unitType) {
  return Ov000_gUnitStats[unitType].movement;
}
