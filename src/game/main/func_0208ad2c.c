#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_021614c0;
extern MenuButtonTemplate data_02161498;

void func_0208ad2c(void) {
  MenuButton_Add(0, 0, &data_021614c0, 14);
  MenuButton_Add(0, 0, &data_02161498, 15);
}
