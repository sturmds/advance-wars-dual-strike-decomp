#include "types.h"
#include "game/Unk_02080440.h"

extern s16 gSidePanelRestX[];

void PowerPanel_DrawCurrentPlayer(int x);

/* Puts the side panel on the right, at its resting x (gSidePanelRestX[1]). */
void SidePanel_SnapRight(void) {
  MAP_STATE->sidePanelRight = 1;
  MAP_STATE->sidePanelX = gSidePanelRestX[1];
  PowerPanel_DrawCurrentPlayer(MAP_STATE->sidePanelX);
}
