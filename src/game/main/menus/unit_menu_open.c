#include "types.h"
#include "game/menu_item.h"

void Font_LoadDigits(int baseTile);
void func_020c4018(void);
int UnitMenu_GetPopupX(void);
int UnitMenu_GetPopupY(void);
void PopupMenu_OpenWithSound(MenuItem* items, int x, int y, int arg, int arg2);
void MenuDepth_Push(void);

/* Opens the unit command menu next to the map cursor. */
void UnitMenu_Open(void) {
  int x;

  func_020c4018();
  Font_LoadDigits(0);
  x = UnitMenu_GetPopupX();
  PopupMenu_OpenWithSound(gUnitMenuItems, x, UnitMenu_GetPopupY(), 1, 1);
  MenuDepth_Push();
}
