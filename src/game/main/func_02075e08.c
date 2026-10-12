#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215fbbc;
extern const MenuButtonTemplate data_0215fbe4;

MenuButton* func_02075e08(int selected) {
  int i;

  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Add(0, 0, &data_0215fbbc, 254);
  for (i = 0; i < 6; i++) {
    MenuButton_Add(56, 48 + i * 16, &data_0215fbe4, i);
  }
  return MenuButton_Select(selected);
}
