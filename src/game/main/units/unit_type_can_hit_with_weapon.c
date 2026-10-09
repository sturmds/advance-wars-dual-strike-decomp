#include "types.h"
#include "game/unit_stats.h"

/* Nonzero if a unit type's weapon (0 primary, 1 secondary) can hit a
 * damage chart column. */
int UnitType_CanHitWithWeapon(int unitType, int targetType, int weapon)
{
    return Ov000_gUnitStats[unitType].canHit[weapon][targetType];
}
