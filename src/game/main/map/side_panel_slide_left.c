#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* One step of the side panel moving to the left side. From the right edge it
 * eases out past the screen edge (x 265 and up); it then reappears at x -87 and
 * eases in to 0. */
void SidePanel_SlideLeft(void) {
  if (STATE->sidePanelX >= 192) {
    STATE->sidePanelRight = 0;
    STATE->sidePanelX -= (STATE->sidePanelX - 266) / 2;
    if (STATE->sidePanelX >= 265) {
      STATE->sidePanelX = -87;
    }
  }
  if (STATE->sidePanelX < 0) {
    STATE->sidePanelX -= STATE->sidePanelX / 2;
    if (STATE->sidePanelX >= -1) {
      STATE->sidePanelX = 0;
    }
  }
}
