#include "types.h"
#include "game/Unk_02080440.h"

void InfoPanel_Draw(void);

/* Draws the info panel when the state byte unk_84 is set, then runs the
 * screen's afterInfoPanelDraw hook if it has one. */
void MapScreen_DrawInfoPanel(void) {
  if (data_027c027c->unk_48[data_027c0284]->unk_84 == 0) {
    return;
  }
  InfoPanel_Draw();
  if (data_027c027c->unk_50[data_027c0284]->afterInfoPanelDraw == NULL) {
    return;
  }
  data_027c027c->unk_50[data_027c0284]->afterInfoPanelDraw();
}
