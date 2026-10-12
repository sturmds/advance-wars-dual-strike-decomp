#include "types.h"

BOOL Ai_FindOwnHqTile(s16* tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (Ai_FindOwnHqTile) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa068(void) {
  s16 tile[2];

  if (Ai_FindOwnHqTile(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
