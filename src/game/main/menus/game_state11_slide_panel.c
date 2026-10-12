#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void SidePanel_Place(int mode);
void func_ov000_022af520(void);

/* Handler for game state 11. Slides the side panel away from the cursor, calls
 * func_ov000_022af520, then switches to state 13. */
void Game_State11_SlidePanel(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  func_ov000_022af520();
  STATE->unk_18 = 13;
}
