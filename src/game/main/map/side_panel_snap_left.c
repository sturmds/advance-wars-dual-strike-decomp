#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 gSidePanelRestX[2];

void PowerPanel_DrawCurrentPlayer(int x);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel on the left, at its resting x (gSidePanelRestX[0]). */
void SidePanel_SnapLeft(void) {
  STATE->sidePanelRight = 0;
  STATE->sidePanelX = gSidePanelRestX[0];
  PowerPanel_DrawCurrentPlayer(STATE->sidePanelX);
}
