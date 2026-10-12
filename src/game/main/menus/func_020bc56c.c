#include "types.h"
#include "game/menu_item.h"

extern MenuItem data_02165bcc[];

void ScreenConfig_RunActionHook(void);
void Font_LoadDigits(int baseTile);
void func_020c4018(void);
int func_020bc6b4(void);
int func_020bc6ac(void);
void PopupMenu_OpenWithSound(MenuItem* items, int x, int y, int arg, int arg2);
void MenuDepth_Push(void);

/* Opens the pop-up menu table data_02165bcc. */
void func_020bc56c(void) {
  int x;

  ScreenConfig_RunActionHook();
  Font_LoadDigits(0);
  func_020c4018();
  x = func_020bc6b4();
  PopupMenu_OpenWithSound(data_02165bcc, x, func_020bc6ac(), 1, 0);
  MenuDepth_Push();
}
