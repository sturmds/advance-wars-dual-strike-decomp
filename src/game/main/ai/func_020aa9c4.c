#include "types.h"

BOOL Ai_FindCaptureSiteTile(s16* tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (Ai_FindCaptureSiteTile) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa9c4(void) {
  s16 tile[2];

  if (Ai_FindCaptureSiteTile(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
