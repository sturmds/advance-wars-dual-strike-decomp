#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02160178;

int func_0207a260(void);

/* Button 254 (the one LinkMenu_Exit closes) exists only on the side where func_0207a260() is not 0. */
void LinkMenu_AddBackButton(void) {
  if (func_0207a260() == 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160178, 254);
}
