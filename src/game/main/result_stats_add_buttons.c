#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215a560;
extern const MenuButtonTemplate data_0215a588;
extern const MenuButtonTemplate data_0215a600;

/* Adds the back button (id 0), the right button (2) and the down button (4). */
void ResultStats_AddButtons(void) {
  MenuButton_Add(0, 0, &data_0215a588, 0);
  MenuButton_Add(0, 0, &data_0215a560, 2);
  MenuButton_Add(0, 0, &data_0215a600, 4);
}
