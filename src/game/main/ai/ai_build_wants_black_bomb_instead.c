#include "types.h"
#include "game/ai_site.h"
#include "game/unit_stats.h"

BOOL AiProfile_HasBuildFlag(int mask);
BOOL AiSites_HasType(int siteType);
int AiSite_PickBlackBombAirport(void);
int Ai_CountUnitsOfType(int type);

#define AI_PROFILE_BLACK_BOMBS 2
/* True when the AI profile allows Black Bombs (bit 2 of the byte AiProfile_HasBuildFlag
 * tests), the player has no Black Bomb, and a target is ready: with a
 * unitType, AiSite_PickBlackBombAirport finds a site and the player has a
 * unit of that type; with 0, a site of type 2 exists. */
BOOL AiBuild_WantsBlackBombInstead(int unitType) {
  if (!AiProfile_HasBuildFlag(AI_PROFILE_BLACK_BOMBS)) {
    return FALSE;
  }
  if (unitType != 0) {
    if (AiSite_PickBlackBombAirport() == -1) {
      return FALSE;
    }
  } else if (!AiSites_HasType(SITE_AIRPORT)) {
    return FALSE;
  }
  if (Ai_CountUnitsOfType(UNIT_BLACK_BOMB) != 0) {
    return FALSE;
  }
  if (unitType != 0 && Ai_CountUnitsOfType(unitType) == 0) {
    return FALSE;
  }
  return TRUE;
}
