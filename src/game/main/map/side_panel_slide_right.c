#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* One step of the side panel moving to the right side, the mirror of
 * SidePanel_SlideLeft. From the left edge it eases out past the screen edge
 * (x -86 and down); it then reappears at x 266 and eases in to 192. */
void SidePanel_SlideRight(void) {
  if (STATE->sidePanelX <= 0) {
    STATE->sidePanelRight = 1;
    STATE->sidePanelX -= (STATE->sidePanelX + 87) / 2;
    if (STATE->sidePanelX <= -86) {
      STATE->sidePanelX = 266;
    }
  }
  if (STATE->sidePanelX > 192) {
    STATE->sidePanelX -= (STATE->sidePanelX - 192) / 2;
    if (STATE->sidePanelX <= 193) {
      STATE->sidePanelX = 192;
    }
  }
}
