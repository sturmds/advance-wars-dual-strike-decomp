#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_Place(int mode);
void MapScript_RunList04(void);

/* Handler for game state 11. Slides the side panel away from the cursor, calls
 * MapScript_RunList04, then switches to state 13. */
void Game_State11_SlidePanel(void) {
  SidePanel_Place(SIDE_PANEL_SLIDE_AWAY);
  MapScript_RunList04();
  MAP_STATE->unk_18 = 13;
}
