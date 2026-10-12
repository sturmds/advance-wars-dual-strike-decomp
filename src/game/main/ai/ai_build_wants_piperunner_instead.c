#include "types.h"
#include "game/unit_stats.h"

BOOL AiProfile_HasBuildFlag(int mask);
int AiSite_PickPiperunnerSite(void);
int Ai_CountUnitsOfType(int type);

#define AI_PROFILE_PIPERUNNERS 1

/* True when the AI profile allows Piperunners (bit 1 of the byte AiProfile_HasBuildFlag
 * tests), a site wants one, and the player has more units of unitType than
 * Piperunners. */
BOOL AiBuild_WantsPiperunnerInstead(int unitType) {
  if (!AiProfile_HasBuildFlag(AI_PROFILE_PIPERUNNERS)) {
    return FALSE;
  }
  if (AiSite_PickPiperunnerSite() == -1) {
    return FALSE;
  }
  return Ai_CountUnitsOfType(unitType) > Ai_CountUnitsOfType(UNIT_PIPERUNNER);
}
