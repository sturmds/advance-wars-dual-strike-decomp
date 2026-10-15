#include "types.h"
#include "game/Unk_02080440.h"

void PopupMenu_PopState(void);
void MenuDepth_Push(void);

/* Handler for game state 22. When a pop-up menu was closed with its restore
 * flag (data_027c027c->unk_40) set, opens that menu again and clears the
 * flag. Then switches to state 15. */
void Game_State22_ReopenPopup(void) {
  if (*data_027c027c->unk_40 != 0) {
    PopupMenu_PopState();
    MenuDepth_Push();
    *data_027c027c->unk_40 = 0;
    data_027c027c->unk_48[data_027c0284]->unk_18 = 15;
  } else {
    data_027c027c->unk_48[data_027c0284]->unk_18 = 15;
  }
}
