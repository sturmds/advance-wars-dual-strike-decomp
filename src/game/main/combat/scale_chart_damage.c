#include "types.h"
#include "game/math.h"

int DamageChart_GetColumn(int targetType, BOOL targetDived);
int UnitType_GetChartDamage(int unitType, int column, int weapon);

/* Damage chart value of a weapon (0 primary, 1 secondary) against a
 * target type, times firepower in percent. A weapon that can hit the
 * target always deals at least 1. */
int ScaleChartDamage(int firepower, int unitType, int targetType, int weapon,
                     BOOL targetDived) {
  int damage = UnitType_GetChartDamage(
      unitType, DamageChart_GetColumn(targetType, targetDived), weapon);

  if (damage > 0) {
    damage = HwDiv32(damage * firepower, 100);
    if (damage == 0) {
      damage = 1;
    }
    return damage;
  }
  return 0;
}
