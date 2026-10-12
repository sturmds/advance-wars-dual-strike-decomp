#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void SidePanel_SlideLeft(void);
void SidePanel_SlideRight(void);
void PowerPanel_DrawCurrentPlayer(int x);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Per-frame version of SidePanel_SnapAwayFromCursor: the panel slides to the
 * side away from the cursor (SidePanel_SlideRight slides it right, SidePanel_SlideLeft left)
 * instead of jumping, then is placed at its new x. */
void SidePanel_SlideAwayFromCursor(void) {
  Unk_02080440_48* state = STATE;
  s16 screenX = state->cursorX * 16 - state->unk_1c4->scrollX;
  s16 screenY = state->cursorY * 16 - state->unk_1c4->scrollY;

  if (screenY < 80) {
    if (screenX < 128) {
      SidePanel_SlideRight();
    } else {
      SidePanel_SlideLeft();
    }
  } else if (state->sidePanelRight == 1) {
    SidePanel_SlideRight();
  } else {
    SidePanel_SlideLeft();
  }
  PowerPanel_DrawCurrentPlayer(STATE->sidePanelX);
}
