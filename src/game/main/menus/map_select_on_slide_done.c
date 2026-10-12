#include "types.h"
#include "game/menu_button.h"

void MapSelect_AddMapButtons(void);

void MapSelect_OnSlideDone(void) {
  MapSelect_AddMapButtons();
  MenuButton_EnableTouch(254);
  MenuButton_EnableTouch(252);
  MenuButton_EnableTouch(253);
  MenuButton_EnableTouch(251);
  MenuButton_EnableTouch(250);
}
