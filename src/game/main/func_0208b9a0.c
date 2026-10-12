#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_02161744;

void func_0208b9a0(void) {
  MenuButton_Add(0, 0, &data_02161744, 254);
  MenuButton_Select(254);
}
