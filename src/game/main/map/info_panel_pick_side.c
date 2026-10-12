#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void InfoPanel_SlideLeft(void);
void InfoPanel_SlideRight(void);

/* One step of the info panel moving to the side away from the cursor. The
 * cursor's screen position is its tile times 16 less the view scroll. Below
 * the top 32 pixels, the panel goes right when the cursor is in the left
 * half. In the top rows it goes right only when infoPanelRight is 1. */
void InfoPanel_StepAwayFromCursor(void) {
  s16 screenY = STATE->cursorY * 16 - STATE->unk_1c4->scrollY;
  s16 screenX = STATE->cursorX * 16 - STATE->unk_1c4->scrollX;

  if (screenY > 32) {
    if (screenX < 128) {
      InfoPanel_SlideRight();
    } else {
      InfoPanel_SlideLeft();
    }
  } else if (STATE->infoPanelRight == 1) {
    InfoPanel_SlideRight();
  } else {
    InfoPanel_SlideLeft();
  }
}
