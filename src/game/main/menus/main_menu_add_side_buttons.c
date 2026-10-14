#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_02159a9c;
extern MenuButtonTemplate data_02159ac4;

void MainMenu_AddSideButtons(void) {
  MenuButton_Add(0, 0, &data_02159a9c, 253);
  MenuButton_Add(0, 0, &data_02159ac4, 254);
}
