#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when the map cursor is on a unit of the current player that has not
 * waited this turn. A slot's owner is slot / 64 + 1. */
BOOL MapCursor_IsOnIdleOwnUnit(void) {
  int slot = MAP_STATE->mapTiles
                 ->unitAt[MAP_STATE->cursorX +
                          MAP_STATE->mapTiles->rowStart[MAP_STATE->cursorY]];

  if (slot == 0) {
    return FALSE;
  }
  if ((slot >> 6) + 1 != MAP_STATE->currentPlayer) {
    return FALSE;
  }
  return !(MAP_STATE->units[slot].flags & MAP_UNIT_WAITED);
}
