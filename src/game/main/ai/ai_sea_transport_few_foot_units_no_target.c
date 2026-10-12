#include "types.h"

BOOL AiUnit_HasReachableTarget(void);
int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_ScoreReachableUnclaimedEnemyProperties(void);
int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamFootUnitsInRange(void);

/* Tile test for the foot unit and APC routine. FALSE when the acting unit has a
 * reachable target. Otherwise TRUE when the team's foot units reached number
 * less than a limit: the enemy property value reached plus 2 if
 * Ai_CountEnemyGroundUnitsInRange is nonzero, else Ai_ScoreReachableUnclaimedEnemyProperties's result
 * plus 1 (it must be nonzero). */
BOOL AiSeaTransport_FewFootUnitsNoTarget(void) {
  int useProperties;
  int value;
  int footUnits;

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  useProperties = Ai_CountEnemyGroundUnitsInRange();
  if (useProperties) {
    value = Ai_ScoreReachableEnemyProperties();
  } else {
    value = Ai_ScoreReachableUnclaimedEnemyProperties();
  }
  if (value == 0) {
    return FALSE;
  }
  footUnits = Ai_CountTeamFootUnitsInRange();
  if (useProperties == 0) {
    if (footUnits < value + 1) {
      return TRUE;
    }
  } else if (footUnits < value + 2) {
    return TRUE;
  }
  return FALSE;
}
