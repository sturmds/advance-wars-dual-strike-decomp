#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215a538;

MenuButton* func_0203b614(void) {
  return MenuButton_Add(0, 0, &data_0215a538, 0);
}
