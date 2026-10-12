#include "types.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

/* Returns the UnitClass of a unit type for the combat-based stat rows:
 * indirect, direct, the two units that explode, or transport (no attack). */
int GetUnitCombatClass(int unitType) {
  u8 minRange = Ov000_gUnitStats[unitType].minRange;

  if (minRange >= ATTACK_INDIRECT) {
    return UNIT_CLASS_INDIRECT_COMBAT;
  }
  if (minRange == ATTACK_DIRECT) {
    return UNIT_CLASS_DIRECT_COMBAT;
  }
  if (unitType == UNIT_BLACK_BOMB || unitType == UNIT_OOZIUM) {
    return UNIT_CLASS_EXPLODES;
  }
  return UNIT_CLASS_TRANSPORT;
}
