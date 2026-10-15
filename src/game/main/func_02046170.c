#include "types.h"
#include "game/menu_button.h"

void PageStrip_AddButtons(void);

void func_02046170(void) {
  MenuButtons_EnableTouch();
  PageStrip_AddButtons();
}
