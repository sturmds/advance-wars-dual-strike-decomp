#include "types.h"
#include "game/Unk_02080440.h"

BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);

/* TRUE when the current player can see (x, y): a unit on the tile must pass
 * Unit_IsVisibleToCurrentPlayer (hidden units fail it), then the tile must be in sight. */
BOOL Map_IsTileVisible(int x, int y) {
  int unitSlot = MAP_TILES->unitAt[x + MAP_TILES->rowStart[y]];

  if (unitSlot != 0 && !Unit_IsVisibleToCurrentPlayer(unitSlot)) {
    return FALSE;
  }
  return MAP_TILES->inSight[x + MAP_TILES->rowStart[y]] != 0;
}
