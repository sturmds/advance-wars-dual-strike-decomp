#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_SlideAwayFromCursor(void);
void SidePanel_SnapLeft(void);
void SidePanel_SnapRight(void);
void SidePanel_SnapAwayFromCursor(void);

/* Moves the side panel the way mode (SIDE_PANEL_*) says. */
void SidePanel_Place(int mode) {
  switch (mode) {
    case SIDE_PANEL_SLIDE_AWAY:
      SidePanel_SlideAwayFromCursor();
      break;
    case SIDE_PANEL_SNAP_LEFT:
      SidePanel_SnapLeft();
      break;
    case SIDE_PANEL_SNAP_RIGHT:
      SidePanel_SnapRight();
      break;
    case SIDE_PANEL_SNAP_AWAY:
      SidePanel_SnapAwayFromCursor();
      break;
  }
}
