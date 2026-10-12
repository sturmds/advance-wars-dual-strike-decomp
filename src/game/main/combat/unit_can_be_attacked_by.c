#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Whether the attacker's type can attack the target's type, from the
 * category bits in UnitStats. Infantry, Mech, APC, Black Boat, T-Copter
 * and Lander always give FALSE. */
BOOL Unit_CanBeAttackedBy(MapUnit* target, MapUnit* attacker) {
  switch (target->type) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
    case UNIT_APC:
    case UNIT_BLACK_BOAT:
    case UNIT_T_COPTER:
    case UNIT_LANDER:
      return FALSE;
  }
  return (Ov000_gUnitStats[target->type].categoryMask &
          Ov000_gUnitStats[attacker->type].targetMask) != 0;
}
