#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02159d00;

MenuButton* func_02032a9c(void) {
  return MenuButton_Add(0, 0, &data_02159d00, 0);
}
