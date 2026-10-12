#include "types.h"
#include "game/unit_stats.h"

/* Damage chart value of a unit type's weapon (0 primary, 1 secondary)
 * against a chart column, in percent. Callers also use it as a flag:
 * 0 means the weapon cannot hit that column. */
int UnitType_GetChartDamage(int unitType, int column, int weapon) {
  return Ov000_gUnitStats[unitType].chartDamage[weapon][column];
}
