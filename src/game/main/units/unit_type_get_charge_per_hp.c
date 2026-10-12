#include "types.h"
#include "game/unit_stats.h"

/* Power meter charge per shown HP of damage for a unit type. */
int UnitType_GetChargePerHp(int unitType) {
  return Ov000_gUnitStats[unitType].chargePerHp;
}
