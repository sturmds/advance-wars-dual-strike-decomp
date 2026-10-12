#include "types.h"

BOOL Ai_FindReachableAiTarget(s16* tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (Ai_FindReachableAiTarget) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020a9e44(void) {
  s16 tile[2];

  if (Ai_FindReachableAiTarget(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
