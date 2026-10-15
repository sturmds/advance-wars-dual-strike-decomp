#include "types.h"
#include "game/Unk_02080440.h"

void MovePath_Truncate(int count);
void MovePath_FillRangeFromEnd(void);
void RangeMap_TracePath(int x, int y, u8* out);
void MovePath_AppendSteps(void);

/* Cuts the move path back to its first tile, then finds the steps to tile
 * (cursorX, cursorY) into pathSteps and appends them. */
void MovePath_RebuildToTarget(void) {
  MovePath_Truncate(1);
  MovePath_FillRangeFromEnd();
  RangeMap_TracePath(MAP_STATE->cursorX, MAP_STATE->cursorY,
                     MAP_STATE->pathSteps);
  MovePath_AppendSteps();
}
