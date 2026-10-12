#include "types.h"

int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

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
