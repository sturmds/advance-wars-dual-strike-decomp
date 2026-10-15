#include "types.h"
#include "game/ai/ai_units_in_range.h"

/* After Ai_CountEnemyGroundUnitsInRange passes (AiSeaTransport_FewArmedUnitsNoTarget and others
 * also call it): TRUE when the value of the enemy properties the range map
 * reaches is above the number of the team's armed ground units it reaches. */
BOOL AiSeaTransport_PropertiesOutweighArmed(void) {
  int properties;
  int armed;

  if (!Ai_CountEnemyGroundUnitsInRange()) {
    return FALSE;
  }
  properties = Ai_ScoreReachableEnemyProperties();
  armed = Ai_CountTeamArmedGroundUnitsInRange();
  return properties > armed;
}
