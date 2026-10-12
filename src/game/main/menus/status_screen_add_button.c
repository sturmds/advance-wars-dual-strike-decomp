#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_0215fb40;

void StatusScreen_AddButton(void) {
  MenuButton_Add(0, 0, &data_0215fb40, 254);
  MenuButton_Select(254);
}
