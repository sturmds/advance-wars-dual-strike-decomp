#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 gSidePanelRestX[2];

void PowerPanel_DrawCurrentPlayer(int x);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel on the right, at its resting x (gSidePanelRestX[1]). */
void SidePanel_SnapRight(void) {
  STATE->sidePanelRight = 1;
  STATE->sidePanelX = gSidePanelRestX[1];
  PowerPanel_DrawCurrentPlayer(STATE->sidePanelX);
}
