#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the map cursor is on a unit of the current player that has not
 * waited this turn. A slot's owner is slot / 64 + 1. */
BOOL MapCursor_IsOnIdleOwnUnit(void) {
  int slot =
      STATE->mapTiles
          ->unitAt[STATE->cursorX + STATE->mapTiles->rowStart[STATE->cursorY]];

  if (slot == 0) {
    return FALSE;
  }
  if ((slot >> 6) + 1 != STATE->currentPlayer) {
    return FALSE;
  }
  return !(STATE->units[slot].flags & MAP_UNIT_WAITED);
}
