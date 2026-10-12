#include "types.h"

BOOL AiTransport_PickFootUnit(s16* tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (AiTransport_PickFootUnit) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa4a4(void) {
  s16 tile[2];

  if (AiTransport_PickFootUnit(tile)) {
    AiUnit_MoveTowardTile(tile[0], tile[1]);
  }
}
