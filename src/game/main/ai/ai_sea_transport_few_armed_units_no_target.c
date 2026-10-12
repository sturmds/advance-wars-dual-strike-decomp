#include "types.h"

BOOL AiUnit_HasReachableTarget(void);
int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

/* Tile test for the order 0 and 1 routine: like AiSeaTransport_FewArmedUnitsInRange,
 * but FALSE when the acting unit has a reachable target. */
BOOL AiSeaTransport_FewArmedUnitsNoTarget(void) {
  int limit;

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  limit = Ai_CountEnemyGroundUnitsInRange();
  if (limit == 0) {
    return FALSE;
  }
  if (Ai_CountTeamArmedGroundUnitsInRange() < limit + 5) {
    return TRUE;
  }
  return FALSE;
}
