#include "types.h"
#include "game/menu_button.h"

void MapSelect_AddMapButtons(void);

void func_02060f84(void) {
  MapSelect_AddMapButtons();
  MenuButtons_EnableTouch();
}
