#include "types.h"
#include "game/Unk_02080440.h"

void SidePanel_Place(int mode);
void MenuButtons_CloseAll(void);
void MenuDepth_Pop(void);

void func_020bba60(void) {
  SidePanel_Place(SIDE_PANEL_SNAP_LEFT);
  MenuButtons_CloseAll();
  MenuDepth_Pop();
}
