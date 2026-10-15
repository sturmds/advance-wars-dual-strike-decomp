#include "types.h"
#include "game/Unk_02080440.h"

/* One step of the side panel moving to the right side, the mirror of
 * SidePanel_SlideLeft. From the left edge it eases out past the screen edge
 * (x -86 and down); it then reappears at x 266 and eases in to 192. */
void SidePanel_SlideRight(void) {
  if (MAP_STATE->sidePanelX <= 0) {
    MAP_STATE->sidePanelRight = 1;
    MAP_STATE->sidePanelX -= (MAP_STATE->sidePanelX + 87) / 2;
    if (MAP_STATE->sidePanelX <= -86) {
      MAP_STATE->sidePanelX = 266;
    }
  }
  if (MAP_STATE->sidePanelX > 192) {
    MAP_STATE->sidePanelX -= (MAP_STATE->sidePanelX - 192) / 2;
    if (MAP_STATE->sidePanelX <= 193) {
      MAP_STATE->sidePanelX = 192;
    }
  }
}
