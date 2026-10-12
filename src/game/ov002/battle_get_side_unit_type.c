#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

/* Unit type of a side, with the battle-only ids 27 to 32 mapped to a
 * regular type. */
int Battle_GetSideUnitType(int side) {
  u8 type = BATTLE_SIDE_UNIT_TYPE(side);

  if (type < UNIT_TYPE_COUNT) {
    return type;
  }
  switch (type) {
    case 27:
      return UNIT_INFANTRY;
    case 28:
      return UNIT_MECH;
    case 29:
      return UNIT_ANTI_AIR;
    case 30:
      return UNIT_PIPERUNNER;
    case 31:
      return UNIT_STEALTH;
    case 32:
      return UNIT_SUB;
    default:
      return UNIT_INFANTRY;
  }
}
