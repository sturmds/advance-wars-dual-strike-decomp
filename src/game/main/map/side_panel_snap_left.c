#include "types.h"
#include "game/Unk_02080440.h"

extern s16 gSidePanelRestX[];

void PowerPanel_DrawCurrentPlayer(int x);

/* Puts the side panel on the left, at its resting x (gSidePanelRestX[0]). */
void SidePanel_SnapLeft(void) {
  MAP_STATE->sidePanelRight = 0;
  MAP_STATE->sidePanelX = gSidePanelRestX[0];
  PowerPanel_DrawCurrentPlayer(MAP_STATE->sidePanelX);
}
