#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

/* Nonzero when the unit on that side is a ship. 32 is not a unit type in
 * Ov000_gUnitStats; the value is set elsewhere in ov002. */
BOOL Battle_IsSideSea(int side) {
  switch (BATTLE_SIDE_UNIT_TYPE(side)) {
    case UNIT_BLACK_BOAT:
    case UNIT_BATTLESHIP:
    case UNIT_CRUISER:
    case UNIT_LANDER:
    case UNIT_SUB:
    case UNIT_CARRIER:
    case 32:
      return TRUE;
    default:
      return FALSE;
  }
}
