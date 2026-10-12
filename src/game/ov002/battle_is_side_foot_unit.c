#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

/* Infantry, Mech, or type 27 or 28 (not in Ov000_gUnitStats). */
BOOL Battle_IsSideFootUnit(int side) {
  switch (BATTLE_SIDE_UNIT_TYPE(side)) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
    case 27:
    case 28:
      return TRUE;
    default:
      return FALSE;
  }
}
