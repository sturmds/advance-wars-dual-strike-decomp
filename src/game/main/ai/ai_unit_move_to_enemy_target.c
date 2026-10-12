#include "types.h"

BOOL AiUnit_FindEnemyTarget(s16* tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (AiUnit_FindEnemyTarget) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void AiUnit_MoveToEnemyTarget(void) {
  s16 tile[2];

  if (AiUnit_FindEnemyTarget(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
