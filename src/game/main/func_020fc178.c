#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_0216a4c4;

void func_020fc178(void) {
  MenuButton_Add(0, 0, &data_0216a4c4, 254);
  MenuButton_Select(254);
}
