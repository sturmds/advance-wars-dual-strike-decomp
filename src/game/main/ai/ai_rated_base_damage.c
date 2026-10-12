#include "types.h"
#include "game/unit_stats.h"

int UnitType_BestBaseDamage(int unitType, int targetType);

/* Best base damage, raised 1.5 times for a Fighter against a Fighter and 2 times
 * for an Anti-Air against a Fighter, so the AI rates these attacks higher. */
int Ai_RatedBaseDamage(int unitType, int targetType) {
  int damage = UnitType_BestBaseDamage(unitType, targetType);

  if (targetType == UNIT_FIGHTER && unitType == UNIT_FIGHTER) {
    damage = damage * 3 >> 1;
  }
  if (targetType == UNIT_FIGHTER && unitType == UNIT_ANTI_AIR) {
    damage = damage * 2;
  }
  return damage;
}
