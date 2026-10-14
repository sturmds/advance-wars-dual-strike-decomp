#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_0215ac74;
extern MenuButtonTemplate data_0215acec;

void func_02046d68(void) {
  MenuButton_Add(0, 0, &data_0215ac74, 254);
  MenuButton_Add(0, 0, &data_0215acec, 253);
}
