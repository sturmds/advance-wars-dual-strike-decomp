#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void SidePanel_Place(int mode);
void MapScript_RunList04(void);

/* Handler for game state 11. Slides the side panel away from the cursor, calls
 * MapScript_RunList04, then switches to state 13. */
void Game_State11_SlidePanel(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  MapScript_RunList04();
  STATE->unk_18 = 13;
}
