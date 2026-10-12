#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

int TargetKind_GetChartColumn(
    int target); /* damage chart column of a target kind */
int Unit_GetChartDamage(MapUnit* unit, int targetType, int weapon,
                        BOOL targetDived);

/* Damage a unit deals to a target kind (not a unit slot; see
 * Combat_CalcDamageToTarget), with the better of its two weapons. The
 * primary weapon counts only while it has ammo, or when the unit type
 * has no ammo stat. */
int Unit_BestDamageToTargetKind(MapUnit* unit, int target) {
  int primaryDamage;
  int secondaryDamage;

  if (Ov000_gUnitStats[unit->type].ammo != 0 && unit->ammo == 0) {
    primaryDamage = 0;
  } else {
    primaryDamage =
        Unit_GetChartDamage(unit, TargetKind_GetChartColumn(target), 0, FALSE);
  }
  secondaryDamage =
      Unit_GetChartDamage(unit, TargetKind_GetChartColumn(target), 1, FALSE);
  if (primaryDamage > secondaryDamage) {
    return primaryDamage;
  }
  return secondaryDamage;
}
