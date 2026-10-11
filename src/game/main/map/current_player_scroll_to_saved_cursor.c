#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020f5888(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Calls func_020f5888 with the tile saved in the current player's
 * unk_31 and unk_32 (likely the cursor position). */
void CurrentPlayer_ShowSavedCursor(void)
{
    func_020f5888(STATE->players[STATE->currentPlayer].savedCursorX,
                  STATE->players[STATE->currentPlayer].savedCursorY);
}
