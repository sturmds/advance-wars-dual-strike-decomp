#include "types.h"

void Font_LoadDigits(int baseTile);
void PopupMenu_PopState(void);
void MenuDepth_Push(void);

void func_020bc548(void) {
  Font_LoadDigits(0);
  PopupMenu_PopState();
  MenuDepth_Push();
}
