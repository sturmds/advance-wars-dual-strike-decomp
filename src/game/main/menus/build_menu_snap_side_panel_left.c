#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_Place(int mode);
BOOL func_02019bec(void);

/* Script step of the build menu task (data_0216594c): moves the side panel
 * to the left. It also asks func_02019bec and returns either way; needed to
 * match: the compiler keeps the compare. */
void BuildMenu_SnapSidePanelLeft(void) {
  SidePanel_Place(SIDE_PANEL_SNAP_LEFT);
  if (func_02019bec() != FALSE) {
    return;
  }
}
