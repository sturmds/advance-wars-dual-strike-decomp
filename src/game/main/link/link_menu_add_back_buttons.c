#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215ff4c;
extern const MenuButtonTemplate data_0215ff74;

int func_0207a260(void);

/* Adds button 254, then button 253 when func_0207a260 returns 0. */
void LinkMenu_AddBackButtons(void) {
  MenuButton_Add(0, 0, &data_0215ff74, 254);
  if (func_0207a260() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215ff4c, 253);
}
