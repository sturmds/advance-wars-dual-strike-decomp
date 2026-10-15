#include "types.h"
#include "game/Unk_02080440.h"

/* One step of the side panel moving to the left side. From the right edge it
 * eases out past the screen edge (x 265 and up); it then reappears at x -87 and
 * eases in to 0. */
void SidePanel_SlideLeft(void) {
  if (MAP_STATE->sidePanelX >= 192) {
    MAP_STATE->sidePanelRight = 0;
    MAP_STATE->sidePanelX -= (MAP_STATE->sidePanelX - 266) / 2;
    if (MAP_STATE->sidePanelX >= 265) {
      MAP_STATE->sidePanelX = -87;
    }
  }
  if (MAP_STATE->sidePanelX < 0) {
    MAP_STATE->sidePanelX -= MAP_STATE->sidePanelX / 2;
    if (MAP_STATE->sidePanelX >= -1) {
      MAP_STATE->sidePanelX = 0;
    }
  }
}
