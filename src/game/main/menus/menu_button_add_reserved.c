#include "types.h"
#include "game/menu_button.h"

/* The last entry of gMenuButtons; the free-slot search for new buttons stops before it. */
extern MenuButton gMenuButtonReserved;

/* Makes the reserved button, with id 0xff, from template at (x, y). */
MenuButton* MenuButton_AddReserved(int x, int y,
                                   const MenuButtonTemplate* template) {
  return MenuButton_Init(x, y, &gMenuButtonReserved, template, 0xff);
}
