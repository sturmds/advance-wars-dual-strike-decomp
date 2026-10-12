#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);

#define MAP (data_027c027c->unk_48[data_027c0284]->mapTiles)

/* TRUE when the current player can see (x, y): a unit on the tile must pass
 * Unit_IsVisibleToCurrentPlayer (hidden units fail it), then the tile must be in sight. */
BOOL Map_IsTileVisible(int x, int y) {
  int unitSlot = MAP->unitAt[x + MAP->rowStart[y]];

  if (unitSlot != 0 && !Unit_IsVisibleToCurrentPlayer(unitSlot)) {
    return FALSE;
  }
  return MAP->inSight[x + MAP->rowStart[y]] != 0;
}
