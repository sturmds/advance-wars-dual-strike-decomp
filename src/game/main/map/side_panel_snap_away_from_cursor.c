#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_SnapRight(void);
void SidePanel_SnapLeft(void);

/* Puts the side panel on the side away from the cursor when the cursor is in
 * the top 80 pixels of the screen; lower down, the panel keeps its side. */
void SidePanel_SnapAwayFromCursor(void) {
  Unk_02080440_48* state = MAP_STATE;
  s16 screenX = state->cursorX * 16 - state->unk_1c4->scrollX;
  s16 screenY = state->cursorY * 16 - state->unk_1c4->scrollY;

  if (screenY < 80) {
    if (screenX < 128) {
      SidePanel_SnapRight();
    } else {
      SidePanel_SnapLeft();
    }
  } else if (state->sidePanelRight == 0) {
    SidePanel_SnapLeft();
  } else {
    SidePanel_SnapRight();
  }
}
