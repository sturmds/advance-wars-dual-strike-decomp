#include "types.h"

int UnitType_GetChartDamage(int unitType, int column, int weapon);

/* Higher damage chart value of a unit type's two weapons against a
 * target type, used directly as the chart column. */
int UnitType_BestBaseDamage(int unitType, int targetType) {
  int damage = UnitType_GetChartDamage(unitType, targetType, 0);
  int secondary = UnitType_GetChartDamage(unitType, targetType, 1);

  if (damage <= secondary) {
    damage = secondary;
  }
  return damage;
}
