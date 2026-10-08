#include "types.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

extern UnitStats Ov000_gUnitStats[UNIT_TYPE_COUNT];

/* Returns the UnitClass of a unit type for the combat-based stat rows:
 * indirect, direct, the two units that explode, or transport (no attack). */
int CheckUnitType(int unitType)
{
    u8 attackType = Ov000_gUnitStats[unitType].attackType;

    if (attackType >= ATTACK_INDIRECT) {
        return UNIT_CLASS_INDIRECT_COMBAT;
    }
    if (attackType == ATTACK_DIRECT) {
        return UNIT_CLASS_DIRECT_COMBAT;
    }
    if (unitType == UNIT_BLACK_BOMB || unitType == UNIT_OOZIUM) {
        return UNIT_CLASS_EXPLODES;
    }
    return UNIT_CLASS_TRANSPORT;
}
