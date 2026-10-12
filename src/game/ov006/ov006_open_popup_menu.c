#include "types.h"
#include "game/popup_menu.h"

PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                u16 selected, int center);
void func_ov006_02354680(PopupMenuTask* menu);
extern const MenuItem data_ov006_0236af9c[];

void Ov006_OpenPopupMenu(void) {
  func_ov006_02354680(PopupMenu_Create(data_ov006_0236af9c, 2, 3, 0, 0, 0));
}
