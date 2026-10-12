#include "types.h"
#include "game/menu_button.h"

extern MenuButtonTemplate data_0215afb4;

/* Adds the button with id 254 at the origin and selects it. */
void func_0204cc54(void) {
  MenuButton_Add(0, 0, &data_0215afb4, 254);
  MenuButton_Select(254);
}
