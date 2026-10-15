#include "types.h"
#include "game/Unk_02080440.h"

void func_020f5888(int x, int y);

/* Calls func_020f5888 with the tile saved in the current player's
 * unk_31 and unk_32 (likely the cursor position). */
void CurrentPlayer_ShowSavedCursor(void) {
  func_020f5888(MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorX,
                MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorY);
}
