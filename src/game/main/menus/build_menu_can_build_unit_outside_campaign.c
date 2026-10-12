#include "types.h"
#include "game/unit_stats.h"

/* Every unit type except Oozium can be built; the player is not used. */
BOOL BuildMenu_CanBuildUnitOutsideCampaign(int player, int unitType) {
  switch (unitType) {
    case UNIT_OOZIUM:
      return FALSE;
    default:
      return TRUE;
  }
}
